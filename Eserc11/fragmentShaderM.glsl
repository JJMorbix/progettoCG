#version 330 core
in vec4 ourColor ;
out vec4 FragColor;  

//Dichiara un campionatore uniforme id_tex di tipo sampler2D. 
//Questo campionatore viene utilizzato per accedere a una texture 2D.
//Esso contiene le informazioni necessarie per accedere alla texture specificata.
 uniform sampler2D id_tex1;
 uniform sampler2D id_tex2;

 
 in vec2 frag_coord_st; //Questa variabile contiene le coordinate della texture del frammento corrente.


uniform int sceltaShader; 
void main()
{
if  (sceltaShader==0 || sceltaShader==4 )
    FragColor = ourColor;

    if  (sceltaShader==1 || sceltaShader == 5 || sceltaShader == 6)
    // texture  una funzione predefinita in GLSL che viene utilizzata per campionare una texture.
    //In sostanza, prende un campionatore (in questo caso id_tex) e delle coordinate di texture (in questo caso frag_coord_st)
    //e restituisce il colore del texel (elemento di texture) corrispondente a quelle coordinate.
     FragColor= mix(ourColor,texture(id_tex1,frag_coord_st),1.0);
     //FragColor = mix(texture(id_tex1,vec2(frag_coord_st.x,frag_coord_st.y)),texture(id_tex2,vec2(frag_coord_st.x,frag_coord_st.y)), 0.5);

}