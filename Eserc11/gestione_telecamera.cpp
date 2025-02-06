#include "lib.h"
#include "strutture.h"
#include "Utilities.h"
#include "gestione_telecamera.h"

extern GLFWwindow* window;
extern mat4 Projection;
extern int height, width;
 
extern ViewSetup SetupTelecamera;
extern PerspectiveSetup SetupProspettiva;
extern float cameraSpeed;

vec3 slide_vector;

void moveCameraForward()
{
	SetupTelecamera.direction= SetupTelecamera.target - SetupTelecamera.position;
	SetupTelecamera.position += SetupTelecamera.direction * cameraSpeed;
	SetupTelecamera.target = SetupTelecamera.position + SetupTelecamera.direction;

	if (checkCollisions())
		moveCameraBack();
}

void moveCameraBack()
{
	SetupTelecamera.direction = SetupTelecamera.target - SetupTelecamera.position;
	SetupTelecamera.position -= SetupTelecamera.direction * cameraSpeed;
	SetupTelecamera.target = SetupTelecamera.position + SetupTelecamera.direction;

	if (checkCollisions())
		moveCameraForward();
}

void moveCameraLeft()
{
	//Calcolo la direzione perpendicolare alla direzione della camera e l'alto della camera
		// e muovo la camera a sinistra lungo questa direzione
	SetupTelecamera.direction = SetupTelecamera.target - SetupTelecamera.position;
	slide_vector = cross(SetupTelecamera.direction, vec3(SetupTelecamera.upVector)) * cameraSpeed;
	SetupTelecamera.position -= slide_vector;
	SetupTelecamera.target -= slide_vector;  //Questo mantiene il target allineato con la telecamera durante lo spostamento laterale.

	if (checkCollisions())
		moveCameraRight();
}

void moveCameraRight()
{
	//Calcolo la direzione perpendicolare alla direzione della camera e l'alto della camera
	// e muovo la camera a destra lungo questa direzione
	SetupTelecamera.direction = SetupTelecamera.target - SetupTelecamera.position;  //Direzione lungo cui si sposta la telecamera in coordinate del mondo
	slide_vector = cross(SetupTelecamera.direction, vec3(SetupTelecamera.upVector)) * cameraSpeed;
	SetupTelecamera.position += slide_vector;
	SetupTelecamera.target += slide_vector; //Questo mantiene il target allineato con la telecamera durante lo spostamento laterale.

	if (checkCollisions())
		moveCameraLeft();
}



void moveCameraUp()
{
	SetupTelecamera.direction = SetupTelecamera.target - SetupTelecamera.position;
	slide_vector=normalize(cross(SetupTelecamera.direction, SetupTelecamera.upVector));
	vec3 upDirection = cross(SetupTelecamera.direction, slide_vector) * cameraSpeed;
	SetupTelecamera.position -= upDirection;
	SetupTelecamera.target -= upDirection;  //Questo mantiene il target allineato con la telecamera durante lo spostamento laterale.

	if (checkCollisions())
		moveCameraDown();
}

void moveCameraDown()
{
	SetupTelecamera.direction = SetupTelecamera.target - SetupTelecamera.position;
	slide_vector = normalize(cross(SetupTelecamera.direction, vec3(SetupTelecamera.upVector)));
	vec3 upDirection = cross(SetupTelecamera.direction, slide_vector) * cameraSpeed;
	SetupTelecamera.position += upDirection;
	SetupTelecamera.target += upDirection; //Questo mantiene il target allineato con la telecamera durante lo spostamento laterale.

	if (checkCollisions())
		moveCameraUp();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{ 
	if (yoffset < 0)
		
		SetupProspettiva.fovY-=1; //Rotella del mouse indietro
	 else
		SetupProspettiva.fovY += 1;  //Rotella del mouse in avanti

}