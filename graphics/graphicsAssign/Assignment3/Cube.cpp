GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
//GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */
GLfloat light_position[] = {1.0, 1.0, 1.0, 1.0};

//my parameters
GLfloat blackcolor[]={0.0,0.0,0.0,1.0};
GLfloat whitecolor[]={1.0,1.0,1.0,1.0};
GLfloat diffuseCoeff[]={1.0,0.4,0.9,1.0};
GLfloat specularCoeff[]={1.0,1.0,1.0,1.0};
GLfloat Shine=100.0;

//GLfloat light_position[] = {0.0, 0.0, -1.0, 0.0};
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */

GLfloat EyePos[3]={0.0,0.0,5.0};


void
drawBox(void)
{
  int i;

  for (i = 0; i < 6; i++) {
    glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);

    glVertex3fv(&v[faces[i][0]][0]);

    glVertex3fv(&v[faces[i][1]][0]);

    glVertex3fv(&v[faces[i][2]][0]);

    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}
