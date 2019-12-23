#ifndef __GWU_BOIDS__
#define __GWU_BOIDS__

void boidsmove(boid boids[20] ,vec3 leader)
{
	GLfloat coefficient=1500.0f;
	for (int index=0;index<20;index++)
	{
		//following
		boids[index].velocity+=(leader-boids[index].position)/coefficient;

		vec3 sum,average,sum_pos,center;
		for(int i=0;i<20;i++)
		{	
			sum +=boids[i].velocity;
			sum_pos+=boids[index].position;
			//collision		
			if(i != index && (boids[i].position-boids[index].position).magnitude()<4)
				boids[index].velocity-=(boids[i].position-boids[index].position)/coefficient;
		}
		//together
		center=(sum_pos-boids[index].position)/19;
		boids[index].velocity+=(center-boids[index].position)/coefficient;
		//speed
		average=(sum-boids[index].velocity)/19;
		boids[index].velocity+=(average-boids[index].velocity)/coefficient;
		//update
		boids[index].position+= boids[index].velocity*0.4;
		boids[index].updateMatrix();

		glPushMatrix();
		glMultMatrixf(boids[index].matrix);
		glutSolidSphere(0.3,20,20);
		glPopMatrix();
	}	
}

#endif