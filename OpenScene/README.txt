README

# Implementado
* Todo lo obligatorio
* Extras:
    - Parser de archivos de confuguracion escenas, con un selector al inicio de la ejecucion.

# Configuracion de teclado

Una vez elegida una escena, utilize los numeros de teclado 0 a 9 para modificar los objetos y luces en pantalla, identificados por el orden en el que son cargados desde su archivo de configuracion.
Por defecto, la camara esta parada en la raiz de la escena. El nodo '0' es la raiz.

Rotate pitch:   UP/DOWN Arrows 
Rotate yaw:     SPACE
Rotate roll:    RIGHT/LEFT Arrows 
Scale:          +/-
Translate X:    a/d
Translate Y:    w/s
Translate Z:    q/e

On/Off
    p: shader
    L: specular
    z: z-buffer
    m: wireframe
    b: b-culling
    t: lights

# Formato de las escenas

En la carpeta Scenes, se adjuntan los archivos de scene graph para configurar la vista en pantalla.
El formato del arbol es el siguiente:

node
    model RELATIVE_PATH_TO_MODE.obj
    texture RELATIVE_PATH_TO_TEXTURE.png
    position X Y Z
    scale X Y Z
    rotation PITCH YAW ROLL
    diffuse R G B A
    specular R G B A
    shininess VALUE             
    node
        light
        type 0                  (0=direccional, 1=puntual)
        position X Y Z
        scale X Y Z
        rotation X Y Z
        ambient R G B A
        diffuse R G B A
        specular R G B A
    end
end

Abrir un bloque 'node-end' adentro de un 'node' define un hijo del nodo padre. 
La primera linea 'model' o 'light' determina el tipo de nodo.
Todos los valores son requeridos.

Adjuntamos 2 escenas:
    * Scenes/scene1.txt tiene 3 knights cada uno asociado con una caja en posicion de arma, con varias luces tenues
        - La idea es usar los numeros de teclado para cambiar el objeto activo y asi rotar/transladarlo en la escena
          1-2-3 es knight-caja-luz, lo mismo para 4-5-6 y 7-8-9 
    * Scenes/scene2.txt tiene 1 knight 'adentro' de una box, con varias luces en distintas caras internas de la caja
        - La idea es con las flechas de teclado, rotar distintos objetos y luces y notar como cambia el color de las paredes
          1 es la caja (hijo unico de la raiz), 2 es el knight, 3 a 7 son luces de distintos colores en distintas posiciones.

# Resumen de archivos programados 

Modulos provistos por los docentes: framework\color framework\mm framework\canvas glew shader
Los siguientes son necesarios en el makefile:
* obj - parsea archivos .obj y renderiza en pantalla
* texture - carga texturas
* scene - carga escenas desde archivos de configuracion
* main - input de teclado e inicializacion