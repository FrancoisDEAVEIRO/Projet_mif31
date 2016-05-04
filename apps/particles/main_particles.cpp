

#include <Grapic.h>
#include <cmath>
using namespace grapic;

const int NPMAX = 1000;
const int DIMW = 500;
const int DIMG = 64;
const float  friction = 0.99f;

struct Vec2f
{
	float x, y;
};

struct Particle
{
	Vec2f p;
	Vec2f v;
	float m;
};


struct Data
{
	Particle part[NPMAX];
	int np;
};


Vec2f operator+(const Vec2f& a, const Vec2f& b)
{
	Vec2f r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	return r;
}

Vec2f operator-(const Vec2f& a, const Vec2f& b)
{
	Vec2f r;
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	return r;
}

Vec2f operator+=(Vec2f& a, const Vec2f& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

Vec2f operator*(float a, const Vec2f& b)
{
	Vec2f r;
	r.x = a * b.x;
	r.y = a * b.y;
	return r;
}

float norm(const Vec2f v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}


void updateParticleP(Data& d, float dt)		// advect
{
	int i;
	for (i = 0; i < d.np; ++i)
		d.part[i].p = d.part[i].p + dt*d.part[i].v;
}

void collision(Data& d)		// advect
{
	int i;
	for (i = 0; i < d.np; ++i)
	{
		if (d.part[i].p.x < 0)
		{
			d.part[i].p.x = -d.part[i].p.x;
			d.part[i].v.x = -d.part[i].v.x;
			d.part[i].v = friction * d.part[i].v;
		}

		if (d.part[i].p.y < 0)
		{
			d.part[i].p.y = -d.part[i].p.y;
			d.part[i].v.y = -d.part[i].v.y;
			d.part[i].v = friction * d.part[i].v;
			d.part[i].v = friction * d.part[i].v;
		}

		if (d.part[i].p.x >= DIMW)
		{
			d.part[i].p.x = DIMW-(d.part[i].p.x-DIMW);
			d.part[i].v.x = -d.part[i].v.x;
			d.part[i].v = friction * d.part[i].v;
		}

		if (d.part[i].p.y >= DIMW)
		{
			d.part[i].p.y = DIMW - (d.part[i].p.y - DIMW);
			d.part[i].v.y = -d.part[i].v.y;
			d.part[i].v = friction * d.part[i].v;
		}
	}
}

void collisionCyclic(Data& d)		// advect
{
	int i;
	for (i = 0; i < d.np; ++i)
	{
		while(d.part[i].p.x < 0)
		{
			d.part[i].p.x += DIMW;
		}

		while (d.part[i].p.y < 0)
		{
			d.part[i].p.y += DIMW;
		}

		while (d.part[i].p.x >= DIMW)
		{
			d.part[i].p.x -= DIMW;
		}

		while (d.part[i].p.y >= DIMW)
		{
			d.part[i].p.y -= DIMW;
		}
	}
}



void updateParticleV(Data& d, float dt)
{
	int i,j;
	const float g = 9.81f;
	//const Vec2f G = { 0.f, g };
	float dist;
	Vec2f dir;
	Vec2f F;
	for (i = 0; i < d.np; ++i)
	{
	    if (d.part[i].m<=0) continue;
		//F = d.part[i].m * G;
		F.x = 0.f;
		F.y = 0.f;
        for (j = 0; j < d.np; ++j)
        {
            if (i==j) continue;
            dir = d.part[i].p-d.part[j].p;
            dist = norm(dir);
            if (dist<0.01f) dist=0.01f;
            F += -g*(d.part[i].m * d.part[j].m)/(dist*dist) * dir;
        }
        d.part[i].v = d.part[i].v + (dt/d.part[i].m)*F;
	}
}


void init(Data& d)
{
	int i;
	d.np = 5;
	for (i = 0; i < d.np; ++i)
	{
		d.part[i].m = 0.01f;		// 10g
		d.part[i].m = i; //0.01f;		// 10g

		d.part[i].p.x = frand(0,DIMW-1); //DIMW / 2;
		d.part[i].p.y = frand(0,DIMW-1); //2*DIMW / 3;

		d.part[i].v.x = frand(-4.f, 4.f);
		d.part[i].v.y = frand(-4.f, 4.f);
	}
	d.part[0].p.x = DIMW/2;
	d.part[0].p.y = DIMW/2;
	d.part[0].v.x = 0;
	d.part[0].v.y = 0;
	d.part[0].m = 1000000;

	d.part[1].p.x = DIMW/2+70;
	d.part[1].p.y = DIMW/2;
	d.part[1].v.x = 0.f;
	d.part[1].v.y = 5000.f;
	d.part[1].m = 1;

	d.part[2].p.x = DIMW/2;
	d.part[2].p.y = DIMW/2+50;
	d.part[2].v.x = 2000.f;
	d.part[2].v.y = 0;
	d.part[2].m = 0.5;


}


void animate(Data& d)
{
	const float dt = 0.001f;
	updateParticleP(d, dt);
	#if 0
	collision(d);
	#else
	collisionCyclic(d);
	#endif
	updateParticleV(d, dt);
}


void draw(Data& d)
{
	color( 255, 0, 0 );
	int i;
	for (i = 0; i < d.np; ++i)
	{
		//point(d.part[i].p.x, d.part[i].p.y);
		color(200+i, 250 - i, 20);
		rectangleFill(d.part[i].p.x-2, d.part[i].p.y-2, d.part[i].p.x + 2, d.part[i].p.y + 2);
	}
    delay(10);
}


int main(int , char ** )
{
    Data dat;
    bool stop=false;
	winInit("Particles", DIMW, DIMW);
	init(dat);
	while( !stop )
    {
        backgroundColor( 100, 50, 200 );
        winClear();
        draw(dat);
        animate(dat);
        //stop = winManageEvent();
        stop = winDisplay();
    }
    winQuit();
	return 0;
}

