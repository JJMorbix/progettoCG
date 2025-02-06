#pragma once
#include "lib.h"
void event_management(GLFWwindow* window)
{
	//Registrazione delle funzioni di callback che verranno chiamate quando si verificano
	   //  determinati eventi

	   //Implementa la chiusura dell'applicazione premendo il tasto Esc  e la modifica del colore dello sfondo con il tasto F2..
	glfwSetKeyCallback(window, key_callback);

	//Per visualizzare le coordinate del mouse che si muove sulla finestra grafica
	glfwSetCursorPosCallback(window, cursor_position_callback);

	//Per visualizzare le coordinate individuate dal tasto sinistro premuto
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	//per visualizzare le dimensioni della finestra ridimensionata

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Imposta la callback per gli eventi della rotellina
	glfwSetScrollCallback(window, scroll_callback);
}