#define PI 3.14f

#define objLeft 90
#define objRight 110
#define objBottom 0
#define objTop 15
#define twoR 0.5

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                               D E N S I T Y       K E R N E L                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Density_Calculation(float ConstantDensitySumTerm,
                         float ConstantDensityKernelTerm,
                         float eps,
                         float H2,
                         global float4* position,
                         global float* density,
                         local float4* pblock){


    // Id of this work-item in global index space
    int gid = get_global_id(0);
    // Id of this work-item within it's work group
    int tid = get_local_id(0);

    int globalSize = get_global_size(0);
    int localSize = get_local_size(0);
    int numTiles = globalSize/localSize;

    // Zero out the density term of this work-item
    density[gid] = 0.0f;
    density[gid] += ConstantDensitySumTerm;

    float4 thisPosition = position[gid];
    float densityTerm = 0.0f;

	// Outer loop iterates over all the work-group blocks
    for(int i = 0; i < numTiles; ++i)
    {
        // Cache the particle position within the work-group
        pblock[tid] = position[(i * localSize) + tid];

        // synchronize to make sure data is available for processing
        barrier(CLK_LOCAL_MEM_FENCE);

        // Inner loop iterates over the work-items in each work-group, after all positions have been cached
        for(int j = 0; j < localSize; ++j)
        {
            float4 otherPosition = pblock[j];

            float4 deltaPosition = thisPosition - otherPosition;

            float r2 = (deltaPosition.x * deltaPosition.x) + (deltaPosition.y * deltaPosition.y) + (deltaPosition.z * deltaPosition.z);
            float z = (H2 - r2) + eps;

            if(z > 0.0f)
            {
                float rho_ij = ConstantDensityKernelTerm * z * z * z;
                densityTerm += rho_ij;
            }
        }

        // Synchronize so that next tile can be loaded
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    density[gid] += densityTerm;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                           A C C E L E R A T I O N       K E R N E L                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Acceleration_Calculation(float eps,
                              float ConstantDensitySumTerm,
                              float ConstantDensityKernelTerm,
                              float H2,
                              float ReferenceDensity,
                              float InteractionRadius,
                              float C0, float CP, float CV,
                              global float4* position,
                              global float4* velocity_full,
                              global float4* acceleration,
                              global float* density,
                              local float4* pblock){

    Density_Calculation(ConstantDensitySumTerm, ConstantDensityKernelTerm, eps, H2, position, density, pblock);

    // Id of this work-item in global index space
    int gid = get_global_id(0);
    // Id of this work-item within it's work group
   // int tid = get_local_id(0);

    int globalSize = get_global_size(0);
   // int localSize = get_local_size(0);
   // int numTiles = globalSize/localSize;

    // Set acceleration parameters
    acceleration[gid].x = 0.0f;
    acceleration[gid].y = -1.0f;
    acceleration[gid].z = 0.0f;

    float4 thisPosition = position[gid];
    float4 thisVelocity = velocity_full[gid];
    float rhoi = density[gid];
    float accelerationTermX = 0.0f;
    float accelerationTermY = 0.0f;
    float accelerationTermZ = 0.0f;

	for (int i = 0; i < globalSize; ++i)
	{
		float4 deltaPosition = thisPosition - position[i];

		float r2 = (deltaPosition.x * deltaPosition.x) + (deltaPosition.y * deltaPosition.y) + (deltaPosition.z * deltaPosition.z);
		float z = (H2 - r2) + eps;
		if (z > 0.0f && r2 != 0.0f)
		{
			float rhoj = density[i];
			float q = sqrt(r2) / InteractionRadius;
			float u = 1.0f - q;
			float w0 = C0 * (u / rhoi / rhoj);
			float wP = w0 * CP * (rhoi + rhoj - (2 * ReferenceDensity)) * (u / q);
			float wV = w0 * CV;

			float4 deltaVelocity = thisVelocity - velocity_full[i];

			accelerationTermX = (wP * deltaPosition.x) + (wV * deltaVelocity.x);
			accelerationTermY = (wP * deltaPosition.y) + (wV * deltaVelocity.y);
			accelerationTermZ = (wP * deltaPosition.z) + (wV * deltaVelocity.z);

			acceleration[gid].x += accelerationTermX;
			acceleration[gid].y += accelerationTermY;
			acceleration[gid].z += accelerationTermZ;

			acceleration[i].x -= accelerationTermX;
			acceleration[i].y -= accelerationTermY;
			acceleration[i].z -= accelerationTermZ;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                           L E A P F R O G       F U N C T I O N                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LeapfrogIntegrator(
const float4 dt,
global float4* position,
global float4* velocity_full,
global float4* velocity_half,
global float4* acceleration
)
{
    // Id of this work-item in global index space
    int gid = get_global_id(0);
    velocity_half[gid] = velocity_full[gid] + (acceleration[gid] * (dt/2));
    velocity_full[gid] += acceleration[gid] * dt;
    position[gid] += velocity_full[gid] * dt;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                           DAMP - REFLECTION        F U N C T I O N S                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DampenReflectionsX(
global float4* position,
global float4* velocity_full,
global float4* velocity_half,
float Restitution,
float barrier
)
{
	// Id of this work-item in global index space
	int gid = get_global_id(0);

	// Ignore degenerate cases
	//if (velocity_full[gid].x == 0.0f)
	//	return;

	// find time since crossing barrier
	float tbounce = (position[gid].x - barrier) / velocity_full[gid].x;

	// Scale back the distance traveled based on time from collision
	position[gid].x -= velocity_full[gid].x * (1 - Restitution) * tbounce;
	position[gid].y -= velocity_full[gid].y * (1 - Restitution) * tbounce;
	position[gid].z -= velocity_full[gid].z * (1 - Restitution) * tbounce;

	// Reflect the position and velocity according to which axis the penetration occured on
	position[gid].x = 2 * barrier - position[gid].x;
	velocity_full[gid].x = -velocity_full[gid].x;
	velocity_half[gid].x = -velocity_half[gid].x;

	// Damp the velocities
	velocity_full[gid] *= Restitution;
	velocity_half[gid] *= Restitution;
}

void DampenReflectionsY(
	global float4* position,
	global float4* velocity_full,
	global float4* velocity_half,
	float Restitution,
	float barrier
)
{
	// Id of this work-item in global index space
	int gid = get_global_id(0);

	// Ignore degenerate cases
	//if (velocity_full[gid].y == 0.0f)
	//	return;

	// find time since crossing barrier
	float tbounce = (position[gid].y - barrier) / velocity_full[gid].y;

	// Scale back the distance traveled based on time from collision
	position[gid].x -= velocity_full[gid].x * (1 - Restitution) * tbounce;
	position[gid].y -= velocity_full[gid].y * (1 - Restitution) * tbounce;
	position[gid].z -= velocity_full[gid].z * (1 - Restitution) * tbounce;

	// Reflect the position and velocity according to which axis the penetration occured on
	position[gid].y = 2 * barrier - position[gid].y;
	velocity_full[gid].y = -velocity_full[gid].y;
	velocity_half[gid].y = -velocity_half[gid].y;

	// Damp the velocities
	velocity_full[gid] *= Restitution;
	velocity_half[gid] *= Restitution;
}

void DampenReflectionsZ(
	global float4* position,
	global float4* velocity_full,
	global float4* velocity_half,
	float Restitution,
	float barrier
)
{
	// Id of this work-item in global index space
	int gid = get_global_id(0);

	// Ignore degenerate cases
	//if (velocity_full[gid].z == 0.0f)
	//	return;

	// find time since crossing barrier
	float tbounce = (position[gid].z - barrier) / velocity_full[gid].z;

	// Scale back the distance traveled based on time from collision
	position[gid].x -= velocity_full[gid].x * (1 - Restitution) * tbounce;
	position[gid].y -= velocity_full[gid].y * (1 - Restitution) * tbounce;
	position[gid].z -= velocity_full[gid].z * (1 - Restitution) * tbounce;


	// Reflect the position and velocity according to which axis the penetration occured on
	position[gid].z = 2 * barrier - position[gid].z;
	velocity_full[gid].z = -velocity_full[gid].z;
	velocity_half[gid].z = -velocity_half[gid].z;

	// Damp the velocities
	velocity_full[gid] *= Restitution;
	velocity_half[gid] *= Restitution;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                       B O U N D A R Y - R E F L E C T - F U N C T I O N                                  //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ReflectBoundaryConditions(
	global float4* position,
	global float4* velocity_full,
	global float4* velocity_half,
	float Restitution,
	float xMin,
	float xMax,
	float yMin,
	float yMax,
	float zMin,
	float zMax)
{
	// Id of this work-item in global index space
	int gid = get_global_id(0);



	if (position[gid].x < xMin)
		DampenReflectionsX(position, velocity_full, velocity_half, Restitution, xMin);
	else if (position[gid].x > xMax)
		DampenReflectionsX(position, velocity_full, velocity_half, Restitution, xMax);

	if (position[gid].y < yMin)
		DampenReflectionsY(position, velocity_full, velocity_half, Restitution, yMin);
	else if (position[gid].y > yMax)
		DampenReflectionsY(position, velocity_full, velocity_half, Restitution, yMax);

    if (position[gid].z < zMin)
		DampenReflectionsZ(position, velocity_full, velocity_half, Restitution, zMin);
	else if (position[gid].z > zMax)
		DampenReflectionsZ(position, velocity_full, velocity_half, Restitution, zMax);



    float4 pos = position[gid];

//reflection from OBJECT

    if(pos.x > objLeft && pos.x < objRight){
        if(pos.y < objTop){
            if(velocity_full[gid].x > 0) DampenReflectionsX(position, velocity_full, velocity_half, Restitution, objLeft);
            else DampenReflectionsX(position, velocity_full, velocity_half, Restitution, objRight);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                           P A R T I C L E - P L A C E M E N T                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int indicatorFBox(float x, float y, float z){
    return ((x<0.5) && (y<0.5) && (z<0.5));
}

int indicatorFCirc(float x, float y, float z){
    float dx = x-0.5;
    float dy = y-0.5;
    float dz = z-0.5;
    float r2 = dx*dx + dy*dy + dz*dz;
    return(r2 < 0.25*0.25*0.25);
}

void particlePlacement(float h /*particlesize*/ ){
    float hh = h/1.3;

    int count = 0;
    for(float x = 0; x<1; x+=hh){
        for(float y=0; y<1; y+= hh){
            for(float z=0;z<1; z+=hh){
                count += indicatorFBox(x,y,z);
            }
        }
    }

    float4 posBuff[100], velBuff[100];
    int p = 0;
    for(float x=0; x<1; x+=hh){
        for(float y=0; y<1; y+= hh){
            for(float z=0; z<1; z+=hh){
                if(indicatorFBox(x,y,z)){
                    posBuff[p].x = x;
                    posBuff[p].y = y;
                    posBuff[p].z = z;
                    ++p;
                }
            }
        }
    }
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                           T H E  - F I N A L - S P H - K E R N E L                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



void kernel SPH_kernel(
                       float dt,
                       float eps,
                       float Restitution,
                       float ConstantDensitySumTerm,
                       float ConstantDensityKernelTerm,
                       float H2,
                       float ReferenceDensity,
                       float InteractionRadius,
                       float C0,
                       float CP,
                       float CV,
                       float xMin,
                       float xMax,
                       float yMin,
                       float yMax,
                       float zMin,
                       float zMax,
                       global float4* position,
                       global float4* velocity_half,
                       global float4* velocity_full,
                       global float4* acceleration,
                       global float* density,
                       local float4* pblock
)
{
    const float4 dt4 = (float4)(dt,dt,dt,0.0f);

    Acceleration_Calculation(eps, ConstantDensitySumTerm, ConstantDensityKernelTerm, H2, ReferenceDensity, InteractionRadius, C0, CP, CV,
                             position, velocity_full, acceleration, density, pblock);

   LeapfrogIntegrator(dt4, position, velocity_full, velocity_half, acceleration);

   ReflectBoundaryConditions(position, velocity_full, velocity_half, Restitution, xMin, xMax, yMin, yMax, zMin, zMax);

}

