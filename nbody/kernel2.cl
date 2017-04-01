__kernel void nBody_kern( float dt1, float eps,
			__global float4* pos_old,
			__global float4* pos_new,
			__global float4* vel,
			__local float4* pblock
			)
{
	float4 dt = (float4)(dt1, dt1, dt1, dt1);

	//optimization for reseting >> set factor = 1.
	int factor = 2;

	int global_id = get_global_id(0); //global thread id gti
	int local_id = get_local_id(0);   //local thread id ti
	int global_size = get_global_size(0); //n
	int local_size = get_local_size(0); //size of 1 block nt
	int number_of_blocks = global_size/local_size;		    //no of blocks nb
    int ib;
	float4 p = pos_old[global_id];
	float4 v = vel[global_id];
	float4 a = (float4)(0.0f, 0.0f, 0.0f, 0.0f);

	//for each <n=factor> blocks
	for(ib=0; ib<number_of_blocks; ib+=factor){

		//cache <n=factor> particles position
		pblock[local_id] = pos_old[(ib * local_size + local_id)];
		pblock[local_id + local_size] = pos_old[ib * local_size + local_id + local_size]; //optimization >> remove this to reset
		barrier(CLK_LOCAL_MEM_FENCE);

		//for each cached particle position
		for(int j=0; j<local_size*factor; j++){
			float4 p2 = pblock[j];
			float4 d = p2 - p;
			float invr = rsqrt( d.x*d.x + d.y*d.y + d.z*d.z + eps );
			float f = p2.w * invr * invr * invr;
			a += f * d;
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}
	p += (dt * v) + (0.5f * dt * dt * a);
	v += dt * a;
	pos_new[global_id] = p;
	vel[global_id] = v;

}
