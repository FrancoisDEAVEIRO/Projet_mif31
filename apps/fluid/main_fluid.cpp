// some doc:
// https://software.intel.com/en-us/articles/fluid-simulation-for-video-games-part-15


#include <cmath>
#include <float.h>
#include <Grapic.h>
using namespace grapic;

const int NPMAX = 100000;
const int DIMW = 512;
const int DIMG = 128;
const float FRICTION = 0.f;
const float VISCOSITY = 0.01f;
const float PARTICLE_WEIGHT = 5.5f;
const float PRESSURE_STIFFNESS = 1.f;
const float RHO0 = PARTICLE_WEIGHT * (DIMW/DIMG) * (DIMW/DIMG);
const float DT = 0.1f;

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

typedef float Array2D[DIMG][DIMG];

struct Data
{
	int np;
	Particle part[NPMAX];
	Array2D rho;		// density
	Array2D pressure;
	Image im_rho;
	Image im_pressure;
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

Vec2f operator*(float a, const Vec2f& b)
{
	Vec2f r;
	r.x = a * b.x;
	r.y = a * b.y;
	return r;
}

Vec2f operator/(const Vec2f& b, float a)
{
	Vec2f r;
	r.x = b.x / a;
	r.y = b.y / a;
	return r;
}

float norm(const Vec2f v)
{
	return sqrt(v.x*v.y + v.y*v.y);
}


Vec2f worl2grid(float x, float y)
{
	Vec2f r;
	r.x = (x / DIMW) * (DIMG - 1);
	r.y = (y / DIMW) * (DIMG - 1);
	assert(r.x >= 0);
	assert(r.y >= 0);
	assert(r.x < DIMG);
	assert(r.y < DIMG);
	return r;
}


void array2dInit(Array2D& d, const Array2D& src)
{
	int i, j;
	for (i = 0; i < DIMG; ++i)
		for (j = 0; j < DIMG; ++j)
			d[i][j] = src[i][j];
}


void array2dInit(Array2D& d)
{
	int i, j;
	for (i = 0; i < DIMG; ++i)
		for (j = 0; j < DIMG; ++j)
			d[i][j] = 0.f;
}

float& array2dGet(Array2D& d, int x, int y)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < DIMG);
	assert(y < DIMG);
	return d[x][y];
}

float array2dGet(const Array2D& d, int x, int y)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < DIMG);
	assert(y < DIMG);
	return d[x][y];
}

float array2dGetF(Array2D& d, float x, float y)
{
	if (x > DIMG - 1) x = DIMG - 1 - 0.001f;
	if (y > DIMG - 1) y = DIMG - 1 - 0.001f;
	if (x < 0) x = 0.f;
	if (y < 0) y = 0.f;

	int X = int(x);
	int Y = int(y);

	if ((X + 1 >= DIMG) || (Y + 1 >= DIMG)) return array2dGet(d,X,Y);
	float IX_b = (x - X)*array2dGet(d,X + 1,Y) + (X + 1 - x)*array2dGet(d,X,Y);
	float IX_h = (x - X)*array2dGet(d,X + 1,Y + 1) + (X + 1 - x)*array2dGet(d,X,Y + 1);

	return (y - Y)*IX_h + (Y + 1 - y)*IX_b;
}

void array2dAdd(Array2D& de, float x, float y, float v)
{
	int X = int(x);
	int Y = int(y);

	float g = (1 - x + X) * v;
	float d = (x - X) * v;
	float bg = (1 - y + Y)*g;
	float hg = (y - Y)*g;
	float bd = (1 - y + Y)*d;
	float hd = (y - Y)*d;

	array2dGet(de,X,Y) += bg;
	if (X+1<DIMG) array2dGet(de, X + 1,Y) += bd;
	if (Y+1<DIMG) array2dGet(de, X, Y + 1) += hg;
	if (X+1<DIMG && Y+1<DIMG) array2dGet(de, X + 1,Y + 1) += hd;
}


Vec2f array2dGrad(Array2D& v, float xw, float yw)
{
	Vec2f gr;
	Vec2f pg = worl2grid(xw, yw);
	float x, y;
	x = pg.x;
	y = pg.y;
	float g, d, h, b;
	g = array2dGetF(v, x - 1, y);
	d = array2dGetF(v, x + 1, y);
	h = array2dGetF(v, x, y + 1);
	b = array2dGetF(v, x, y - 1);
	gr.x = (d - g) / 2.f;
	gr.y = (h - b) / 2.f;

	//if (fabs(gr.x) < 0.001) gr.x = 0;
	//if (fabs(gr.y) < 0.001) gr.y = 0;

	return gr;
}



void computeDensityFromParticles(Array2D& d, Particle part[NPMAX], int n)
{
	int i;
	Vec2f pg;
	array2dInit(d);
	for (i = 0; i < n; ++i)
	{
		pg = worl2grid( part[i].p.x, part[i].p.y);
		array2dAdd(d, pg.x, pg.y, part[i].m);
	}
}


void array2dToImage(Array2D& d, Image& im)
{
	unsigned char v;
	int i, j;
	float m, M;
	m = FLT_MAX;
	M = -FLT_MAX;
	for (i = 0; i < DIMG; ++i)
		for (j = 0; j < DIMG; ++j)
		{
			if (array2dGet(d, i, j) < m) m = array2dGet(d, i, j);
			if (array2dGet(d, i, j) > M) M = array2dGet(d, i, j);
		}
	printf("min=%f max=%f\n", m, M);
	for (i = 0; i < DIMG; ++i)
		for (j = 0; j < DIMG; ++j)
		{
			v = static_cast<unsigned char>( 255.f*(array2dGet(d,i,j)-m)/(M-m) );
			image_set(im, i, j, v,v,v,255 ) ;
		}
}

bool isCoordValidGrid(float x, float y)
{
	return (x >= 0 && y >= 0 && x < DIMG && y < DIMG);
}

void computePressure(Array2D& p, const Array2D& d)
{
	int i, j;
	float presure;
	for (i = 0; i < DIMG; ++i)
		for (j = 0; j < DIMG; ++j)
		{
			presure = PRESSURE_STIFFNESS  * (array2dGet(d, i, j) - RHO0);
			if (presure < 0) presure = 0.f;
			array2dGet(p, i, j) = presure;
		}
}

void updateParticleP(Data& d, float dt)		// advect
{
	int i;
	for (i = 0; i < d.np; ++i)
	{
		d.part[i].p = d.part[i].p + dt*d.part[i].v;
	}
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
			d.part[i].v = FRICTION * d.part[i].v;
		}

		if (d.part[i].p.y < 0)
		{
			d.part[i].p.y = -d.part[i].p.y;
			d.part[i].v.y = -d.part[i].v.y;
			d.part[i].v = FRICTION * d.part[i].v;
		}

		if (d.part[i].p.x >= DIMW)
		{
			d.part[i].p.x = DIMW-(d.part[i].p.x-DIMW);
			d.part[i].v.x = -d.part[i].v.x;
			d.part[i].v = FRICTION * d.part[i].v;
		}

		if (d.part[i].p.y >= DIMW)
		{
			d.part[i].p.y = DIMW - (d.part[i].p.y - DIMW);
			d.part[i].v.y = -d.part[i].v.y;
			d.part[i].v = FRICTION * d.part[i].v;
		}
	}
}


void updateParticleV(Data& d, float dt)
{
	int i;
	Vec2f G = { 0.f, 9.81f };
	//G.x = 9.81f*cos(temps());
	for (i = 0; i < d.np; ++i)
	{
		d.part[i].v = d.part[i].v
						- dt * G
						- array2dGrad(d.pressure, d.part[i].p.x, d.part[i].p.y) / d.part[i].m
						- VISCOSITY*d.part[i].v;
	}
}





void init(Data& d)
{
	int i,j;
	d.np = 0;

	for (i = 0; i < 100; ++i)
		for (j = 0; j < 200; ++j)
		{
			d.part[d.np].m = PARTICLE_WEIGHT;

			d.part[d.np].p.x = DIMW-1-i;
			d.part[d.np].p.y = j;

			d.part[d.np].v.x = 0.f; // frand(-2.f, 2.f);
			d.part[d.np].v.y = 0.f; // frand(-0.f, 8.f);

			++d.np;
			assert(d.np < NPMAX);
		}


	printf("Number of particles=%d", d.np);
	d.im_rho = image(DIMG, DIMG);
	d.im_pressure = image(DIMG, DIMG);
	computeDensityFromParticles(d.rho, d.part, d.np);
	computePressure(d.pressure, d.rho);
}


void draw(Data& d)
{
	color(255, 0, 0);

	if (isKeyPressed(SDL_SCANCODE_D))
	{
		array2dToImage(d.rho, d.im_rho);
		image_draw(d.im_rho, 0, 0, DIMW, DIMW);
	}
	else if (isKeyPressed(SDL_SCANCODE_A))
	{
		array2dToImage(d.pressure, d.im_pressure);
		image_draw(d.im_pressure, 0, 0, DIMW, DIMW);
	}
	else
	{
		color(255, 0, 0);
		int i;
		for (i = 0; i < d.np; ++i)
		{
			//point(d.part[i].p.x, d.part[i].p.y);
			//color(200, d.part[i].v.x*100, d.part[i].v.y * 100);
			//color(200, i, 140);
			color(20, 50, 240);
			rectangleFill(d.part[i].p.x - 1, d.part[i].p.y - 1, d.part[i].p.x + 1, d.part[i].p.y + 1);
		}
	}

}


void animate(Data& d)
{
	updateParticleP(d, DT);
	collision(d);
	computeDensityFromParticles(d.rho, d.part, d.np);
	computePressure(d.pressure, d.rho);

#if 0
	static Density tmp;
	densityInit(tmp, d.density);
	densityDiffuse(d.density, tmp);
#endif

	updateParticleV(d, DT);
}




int main(int , char ** )
{
	Data* pdat = new Data;
	Data& dat = *pdat;
    bool bStop=false;
    bool bPause=false;
	winInit("Fluid", DIMW, DIMW);
	init(dat);
	while( !bStop )
    {
        backgroundColor( 100, 150, 250 );
        winClear();
        //bStop = winManageEvent();
       	if (isKeyPressed(SDL_SCANCODE_R)) init(dat);
       	if (isKeyPressed(SDL_SCANCODE_P)) { bPause = !bPause; delay(100); }

        draw(dat);
        if (!bPause) animate(dat);
        else print(10, 200, "PAUSE");

        bStop = winDisplay();
    }
    winQuit();
	return 0;
}

