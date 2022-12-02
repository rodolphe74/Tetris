uniform float time;
uniform float timems;
uniform vec2 mouse;
uniform vec2 resolution;



uniform int maxIndex;
uniform vec3 pos[1000];
// uniform vec3 size[1000];

struct Particle{
	vec3 pos, speed;
	vec4 color;
	float size;
	float life; // Remaining life of the particle. if < 0 : dead and unused.
};


const int MaxParticles = 100;
Particle ParticlesContainer[MaxParticles];
int LastUsedParticle = 0;


// Finds a Particle in ParticlesContainer which isn't used yet.
// (i.e. life < 0);
int FindUnusedParticle(){

    for(int i=LastUsedParticle; i<MaxParticles; i++){
        if (ParticlesContainer[i].life < 0){
            LastUsedParticle = i;
            return i;
        }
    }

    for(int i=0; i<LastUsedParticle; i++){
        if (ParticlesContainer[i].life < 0){
            LastUsedParticle = i;
            return i;
        }
    }

    return 0; // All particles are taken, override the first one
}



float rand(float co){
    return fract(sin(dot(vec2(co ,co ) ,vec2(12.9898,78.233))) * 43758.5453);
}

float rand_range(float seed, float low, float high) {
	return low + (high - low) * rand(seed);
}




void drawParticle(vec2 coord, float r, float s)
{
    vec2 uv = gl_FragCoord.xy / resolution.xy;	// 0 <> 1
	vec2 m = coord / resolution; // 0 <> 1
	m.x = 1 - m.x;	// x axis the same as uv
	m.x -=0.5;	// -0.5 <> 0.5
    m.y -=0.5;
	uv -= 0.5;	// -0.5 <> 0.5
	uv += m;
	uv.x *= resolution.x / resolution.y;	// compensate screen ratio

	float d = length(uv);
	// float r = 0.03;
	float c = smoothstep(r, r - s, d);
	
    // float c = d;
	// if (d < .3)  c = 1.0; else c = 0;

	gl_FragColor += gl_Color * vec4(vec3(c), 1.0);
}

void createParticles(float x1, float y1, float x2, float y2, int count, float size) {

    float lenX = x2 - x1;
    float lenY = y2 - y1;

    float start = x1;
    float stp = lenX / float(count);
    for (int i = 0; i < count; i++) {
        
        drawParticle(vec2(start, lenY /2), size, 0.05);
        start += stp;

    }

}


/*
void main( void ) 
{	
//    if(gl_FragCoord.x < 400)
//        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); 
//    else
//        gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);



	vec2 uv = gl_FragCoord.xy / resolution.xy;	// 0 <> 1

	vec2 m = mouse / resolution; // 0 <> 1
	m.x = 1 - m.x;	// x axis the same as uv
	m.x -=0.5;	// -0.5 <> 0.5

	uv -= 0.5;	// -0.5 <> 0.5
	uv += m;

	uv.x *= resolution.x / resolution.y;	// compensate screen ratio

	float d = length(uv);
	float r = 0.03;
	float c = smoothstep(r, r - 0.05, d);
	// float c = d;

	// if (d < .3)  c = 1.0; else c = 0;

	// gl_FragColor = vec4(uv.y, 0., 0., 1.0);
	gl_FragColor = gl_Color * vec4(vec3(c), 1.0);
}
*/


//struct Particle{
//	vec3 pos, speed;
//	float r,g,b,a; // Color
//	float size, angle, weight;
//	float life; // Remaining life of the particle. if < 0 : dead and unused.
//};


Particle createParticle(vec2 pos, float seed)
{
	Particle p;
	p.pos.x = pos.x;
	p.pos.y = pos.y;
	p.pos.z = 0.0;

	p.speed.x = rand_range(seed, 1.0, 5.0);
	p.speed.y = rand_range(seed, 1.0, 5.0);

	p.size = 0.005;
	p.life = 500;
	return p;
}



void main( void ) 
{	
//
//	drawParticle(vec2(400, 200), 0.03);
//	drawParticle(vec2(390, 200), 0.03);
//    createParticles(10.0, 10.0, 100.0, 60.0, 3, 0.03);

    //for (int i = 0; i < 200; i++) {

        drawParticle(vec2(100 + int(timems/8.0), 200.0), 0.005, 0.008);


        // gl_FragColor = gl_Color * vec4(vec3(0, 0, 0), 1.0);
    //}

//
	for (int i = 0; i < maxIndex; i++) {
		vec3 currentPos = pos[i];
		// drawParticle(vec2(currentPos.x, currentPos.y), 0.005, 0.008);
	}


	/*
	int ParticlesCount = 0;
    for(int i=0; i<MaxParticles; i++) {

        Particle p = ParticlesContainer[i]; // shortcut
	    if(p.life > 0.0){


        // Decrease life
        p.life -= 10;
        if (p.life > 0.0) {
				drawParticle(vec2(200.0, 200.0), 0.01 , 0.008);

            // Simulate simple physics : gravity only, no collisions
            p.speed += vec3(0.0, -9.81, 0.0) * 10 * 0.5;
            p.pos += p.speed * 10;

			drawParticle(vec2(p.pos.x, p.pos.y), 0.005, 0.008);
        }
    }
	*/


	// Generate 10 new particule each millisecond,
	// but limit this to 16 ms (60 fps), or if you have 1 long frame (1sec),
	// newparticles will be huge and the next frame even longer.
	
	// each 32 ms
	/*
	if (int(timems) % 32 == 0 && LastUsedParticle < MaxParticles) {
	
		for (int i = 0; i < 10; i++) {
			Particle newP;
			newP.pos = vec3(100, 100, 0);
			// newP.speed = vec3(8.0 + rand_range(timems, 0.0, 4.0), 4.0 + rand_range(timems, 0.0, 4.0), 0.0);
			newP.speed = vec3(8.0, 4.0, 0.0);
			newP.color = vec4(1.0, 0.0, 0.0, 1.0);
			newP.size = 0.005;
			newP.life = 50000.0;
			ParticlesContainer[i + LastUsedParticle] = newP;
			LastUsedParticle += 10;
		}
	}


	int ParticlesCount = 0;
    for(int i=0; i<MaxParticles; i++){

        Particle p = ParticlesContainer[i]; // shortcut

        if(p.life > 0.0){


            // Decrease life
            p.life -= 10;
            if (p.life > 0.0){
					drawParticle(vec2(200.0, 200.0), 0.1 , 0.008);

                // Simulate simple physics : gravity only, no collisions
                p.speed += vec3(0.0, -9.81, 0.0) * 10 * 0.5;
                p.pos += p.speed * 10;

				drawParticle(vec2(p.pos.x, p.pos.y), 0.005, 0.008);
            }
        }
    }
	*/


}
