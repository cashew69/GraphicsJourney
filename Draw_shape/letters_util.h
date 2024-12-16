#ifndef letters_util   /* Include guard */
#define int display_alphabet(float, float , float , float , char );

int display_alphabet(float originx, float originy, float width, float height, char alphabet)
{
	switch (alphabet) {
		case 'H' || 'h':
			glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 0.0f);

			glVertex2f(originx, originy);
			glVertex2f(originx, height);

			glVertex2f(originx, height/2);
			glVertex2f(width, height/2);

			glVertex2f(width, originx);
			glVertex2f(width, height);
			
			glEnd();
			break;

	}

	return(0);
}

int display_string()
{
}

#endif 
