#include "animatedBackground.h"

void
AnimatedBackground::renderBackground(const float* shape,
                                     const size_t size,
                                     const float* material)
{
    // DEBUG
    //printf("Amp:%f\n", m_unPeuDeRhythme);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);  // Enable lighting
    glEnable(GL_LIGHT0);    // Enable light #0
    glEnable(GL_LIGHT1);    // Enable light #1
    glEnable(GL_NORMALIZE); // Automatically normalize normals

    glViewport(0, 0, WINDOW_W, WINDOW_H);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, WINDOW_W / (float)WINDOW_H, 1.0, 200);

    // Add ambient light
    GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    // Add positioned light
    GLfloat lightColor0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    // Add directed light
    GLfloat lightColor1[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    // Coming from the direction (-1, 0.5, 0.5)
    GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(.0f, .122f, .596f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    for (int y = -8; y < 8; y++) {
        for (int x = -12; x < 12; x++) {
            glPushMatrix();

            glTranslatef(x * 4 + cos(m_fcountAngle) * 16,
                         y * 4 + sin(m_fcountAngle) * 3,
                         -30 + cos(m_fcountAngle) * 8);
            float shiftAngle = ((float)x * (float)y) * 2.0f;
            glRotatef(m_fa + shiftAngle, 0, 1, 0);
            // glRotatef(m_fa / 2 + shiftAngle, 0, 0, 1);

            GLfloat mat_ambient[] = { material[0], material[1], material[2] };
            GLfloat mat_diffuse[] = { material[3], material[4], material[5] };
            GLfloat mat_specular[] = { material[6], material[7], material[8] };
            GLfloat mat_shininess[] = { material[9] * 128 };

            glColor3f(1.0f, 1.0f, 1.0f);

            glEnable(GL_COLOR_MATERIAL);
            glColorMaterial(GL_FRONT, GL_SPECULAR);

            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);

            glVertexPointer(3, GL_FLOAT, 6 * sizeof(GLfloat), shape);
            glNormalPointer(GL_FLOAT, 6 * sizeof(GLfloat), shape + 3);
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei)size / 6);

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);

            glPopMatrix();
        }
    }

    glPopMatrix();

    m_fa += 0.5f;
    // m_fa += (.5f + (m_unPeuDeRhythme / INT16_MAX) * 40.0f);
    m_fcountAngle += (0.002f + (m_unPeuDeRhythme / INT16_MAX) * 0.02f);
}
