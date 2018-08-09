--------------------------------------------------------------------------------
-- Notas de uso:
--------------------------------------------------------------------------------
Árbol de carpetas:
------------------------------------------
Carpeta del juego/
				  maxscript/
						    autoboot/
				  Bin/
					  data/
						   materials/
						   meshes/
						   scenes/
						   textures/


------------------------------------------
Pasos para instalar los scripts:
------------------------------------------
01. Customize > Configure System Paths... > Additional Startup Scripts > Modify...
02. Seleccionar ~Proyecto~\maxscript\autoboot\
03. Darle a Use Path.
04. Reiniciar Max.
05. Hacer clic derecho en la barra de herramientas de Max > Customize...
06. Ir a la pestaña Toolbars.
07. Darle al botón de la derecha que pone New..., en name introducir "Momentum" y OK.
08. En el desplegable que hay encima del botón New... seleccionar Momentum.
09. Debería aparecer una barra de herramientas vacía, si no es así desmarcar la casilla Hide.
10. Arrastrar la barra vacía a donde te resulte cómodo.
11. En el 2º desplegable de la izquierda llamado Category seleccionar Momentum.
12. Debajo de Action habrá aparecido una línea que reza "Open the momentum utility window".
13. Arrastrar esa línea a la barra de herramientas vacía que hemos creado antes, se creará un botón llamado "Momentum".
14. Cerrar la ventana de Customize User Interface.
15. Si se pulsa el botón "Momentum" se abrirá la ventana con nuestras utilidades.
16. Profit.

-------------------------------
Condiciones de exportación:
-------------------------------
Las layers invisibles o vacías no se exportan.
Lo visible como Editable Mesh.

Nombres de Layers:
·"0" --> Aquí van las meshes visibles. Si tienen mesh hija, será collider convex mesh.
·"altars"
·"breakables"
·"cameras"
·"chrysalis"
·"coins"
·"collisions"
·"enemies" --> Por defecto son voladores, ponerle "melee" en el nombre si no.
·"instancing"
·"lights"
·"mechanisms"
·"movables" --> Si también es pure, tiene que ir en "pure".
·"pure"
·"triggers"
·"trimesh"

El objeto low-res del LOD debe ser padre de un dummy que contiene los objetos
a los que sustituye cuando está activo.

Si un mesh tiene box hija, ambas tienen que tener el pivot en el mismo sitio.
