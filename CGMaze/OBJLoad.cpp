#include "OBJLoad.h"
#include"Maze.h"
#include "defines.h"

OBJLoad::OBJLoad()
{
}

OBJLoad::OBJLoad(string filename)
{

	FILE * input;
	char rBuffer[rdBuffer];
	input = fopen(filename.c_str(), "r");
	if (input == nullptr)
	{
		printf("###Error loading : %s\n", filename.c_str());
	}


	while (fgets(rBuffer, rdBuffer, input) != nullptr)
	{
		char id[rdBuffer];
		sscanf(rBuffer, "%s", id);

		printf("ID string: %s", id);
		
		if (id[0] == '#')
		{
			continue;
		}
		if (strncmp(id, "mtllib", 6)==0)
		{
			continue;
		}

		if (strncmp(id, "o", 1)==0)
		{
			continue;
		}

		if (strncmp(id, "v", 1) == 0 && strlen(id)==1)
		{
			char in;
			double x, y, z;
			sscanf(rBuffer, "%c %lf %lf %lf", &in, &x, &y,&z);
			printf("found VertexCoords: %f %f %f\n", x,y,z);
			vspace v;
			v.x = x;
			v.y = y;
			v.z = z;
			this->vertics.push_back(v);
			continue;
		}
		if (strncmp(id, "vt", 2) == 0)
		{
			char in[rdBuffer];
			double u, v;
			sscanf(rBuffer, "%*s %lf %lf", &u,&v);
			printf("got UVs %lf %lf\n", u,v);
			UV uv;
			uv.u = u;
			uv.v = v;
			uvCoords.push_back(uv);
			continue;
		}
		if (strncmp(id, "vn", 2) == 0)
		{
			char in[rdBuffer];
			double x, y, z;
			sscanf(rBuffer, "%*s %lf %lf %lf", &x,&y,&z);
			printf("Found normies: %lf %lf %lf\n", x,y,z);
			normal n;
			n.x = x;
			n.y = y;
			n.z = z;
			normalList.push_back(n);
			continue;
		}
		if (strncmp(id, "usemtl", 6) == 0){
			continue;
		}
		if (strncmp(id, "s", 1) == 0) { continue; }
		if (strncmp(id, "f",1)==0) {
			int vindex, uvindex, normindex;
			int counter = 0;
			fullVert *vcol[4];

			char *head, *tail;
			head = tail = rBuffer+1;
			for (int i = 0; i < 4; i++)
			{
				vindex = strtol(head, &tail,10);
				if (head == tail) { break; }
				head = tail+1;
				uvindex = strtol(head,&tail,10);
				head = tail+1;
				normindex = strtol(head,&tail,10);
				head = tail+1;

				printf("Got indizes: %d %d %d\n",vindex, uvindex, normindex);
				vcol[i]= new fullVert (this, vindex, uvindex, normindex);
				counter++;
			}
			printf("Got %d verts for face\n", counter);

			if (counter == 3)
			{

				triangle t(*vcol[0], *vcol[1], *vcol[2]);
			
				tris.push_back(t);
				for (int i = 0; i < 3; ++i)
				{
					delete vcol[i];

				}
			}
			if (counter == 4)
			{
				triangle t(*vcol[0], *vcol[1], *vcol[2]);
				tris.push_back(t);
				triangle t1(*vcol[0], *vcol[1], *vcol[2]);
				tris.push_back(t1);

				for (int i = 0; i < 4; ++i)
				{
					delete vcol[i];

				}
			}
		
		}
	}
	

}


OBJLoad::~OBJLoad()
{
}

vspace OBJLoad::getVertex(int index)
{
	if(index-1<vertics.size())
	return this->vertics.at(index-1);

	printf("### index %d over cap %d\n", index, vertics.size());
}

UV OBJLoad::getUv(int index)
{
	if(index-1<uvCoords.size())
	return this->uvCoords.at(index-1);
	printf("### index %d over cap %d\d", index, uvCoords.size());

}

normal OBJLoad::getNormal(int index)
{
	if(index-1<normalList.size())
	return this->normalList.at(index-1);
	printf("### index %d over cap %d\d", index, normalList.size());
}

VertexList OBJLoad::getVertexList()
{
	printf("+++Object contains %d tris\n",tris.size() );
	VertexList VL(vertexCoordinates|vertexColor|UVCoordinates|normals,8);

	for each(triangle t in this->tris)
	{

		;

		VL.addTriangle(t.a.get(),t.b.get(),t.c.get());
	}

	return VL;
}

fullVert::fullVert( OBJLoad *res, int vspaceIndex, int uvIndex, int normalIndex)
{
	this->vx = res->getVertex(vspaceIndex).x;
	this->vy = res->getVertex(vspaceIndex).y;
	this->vz = res->getVertex(vspaceIndex).z;

	this->U = res->getUv(uvIndex).u;
	this->V = res->getUv(uvIndex).v;
	
	this->nx = res->getNormal(normalIndex).x;
	this->ny = res->getNormal(normalIndex).y;
	this->nz = res->getNormal(normalIndex).z;


}

fullVert::fullVert(const fullVert & in)
{

	this->b = in.b;
	this->g=in.g;
	this->nx = in.nx;
	this->ny = in.ny;
	this->nx = in.nx;
	this->r = in.r;
	this->U = in.U;
	this->V = in.V;
	this->vx = in.vx;
	this->vy = in.vy;
	this->vz = in.vz;
}

Vertex fullVert::get()
{
	Vertex x;
	x.b = this->b;
	x.g = this->g;
	x.nx = this->nx;
	x.ny = ny;
	x.nz = nz;
	x.r = r;
	x.u = this->U;
	x.v = this->V;
	this->vx = vx;
	this->vy = vy;
	this->vz = vz;

	return x;
}

triangle::triangle(fullVert a, fullVert b, fullVert c)
{
	this->a = a;
	this->b = b;
	this->c = c;
}
