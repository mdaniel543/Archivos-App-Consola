# Manual de Usuario 

**La aplicacion es totalmente en consola y se utilizan unicamente comandos.** Cada comando contiene parametros obligatorios y opcionales y solo se puede colocar 
un comando por linea.

Si se utliza un parametro que no se reconoce se muestra un mensaje de error. Los parametros se separan por espacios en blanco y si se necesita que algún valor lleve espacios en blanco se encerrará entre
comillas " ".  Y por ultimo la aplicacion no distingue entre mayusculas y minusculas. 
- - -
Los comandos que reconoce la aplicacion son:

- **mkdisk:** este comando crea un archivo que simula un disco duro.
    - size: recibe el tamaño del disco
    - f: indica el ajuste del disco duro (BF, FF, WF), por defecto el ajuste es first fit
    - u: indica la unidad que utilizara size (k, m) por defecto son megabytes
    - path: este indica la ruta donde se crea el archivo
    
    Ejemplo: 
    *mkdisk -Size=3000 -u=K -path=/home/user/Disco1.dk*
    
- **rmdisk:** este parametro elimina un archivo que simula el disco duro.
    - path: indica la ruta donde se encuentra el archivo
    
    Ejemplo:
    *rmDisk -path="/home/mis discos/Disco4.dk"*
    
- **fdisk:** este comando administra las particiones en el archivo que representa al disco duro.
    - size: indica el tamaño de la particion
    - u: indica la unidad que utilizara size (k, m, b) por defecto son kilobytes
    - path: este parámetro será la ruta en la que se encuentra el disco en el que se creara la particion
    - type: indica el tipo de particion que se creara (p,e,l) por defecto son primarias
    - f: indica el ajuste de la particion (BF, FF, WF), por defecto el ajuste worst fit
    - delete: indica que se eliminara una particion con valores de fast, full. Utiliza los parametros name y path
    - name: indicara el nombre de la particion
    - add: se utiliza para agregar o quitar espacio de la particion, puede ser positivo o negativo
    
    Ejemplo: 
    
    *fdisk –Size=300 –path=/home/Disco1.disk –name=Particion1*
    
    *fdisk -name=Particion1 –delete=full -path=/home/Disco1.disk*
    
- **mount:** este comando montara una particion del disco en el sistema e imprimira en consola un resumen de todas las particiones montadas. 
Cada particion montada se identifica por un id(Últimos dos dígitos del Carnet + Numero + Letra) 

    - path: es el que indica la ruta en la que se encuentra el disco que se montará
    - name: indica el nombre de la partición a cargar
    
    Ejemplo:
    *mount -path=/home/Disco1.disk -name=Part1*
    
- **umount:** desmonta una particion del sistema 

    - id: especifica el id de la particion que se desmonta 
    
    Ejemplo:
    *umount -id=121a*
    
- **mkfs:** este comando realiza un formateo completo de la particion.

    - id: indicara el id que se generó con el comando mount.
    - type: indica que formateo se realizara (fast, full) por defecto se formatea full
    - fs: indica el sistema de archivos (2fs, 3fs) por defecto es 2fs.
    
    Ejemplo:
    *mkfs -type=fast -id=581A*
---
Los reportes de igual manera se genera con comandos de la siguiente manera:

- **rep:** reportes generado con graphviz, donde podemos corroborar todo lo ejecutado

    - name: nombre del reporte a generar (mbr, disk)
    - path: indica la ruta con el nombre que tendra el reporte.
    - id: indica el id de la particion que se utilizara, si es reporte sobre el disco se utilizara el disco al que pertenece la particion.
    
    Ejemplo:
    
    *rep –id=561A -Path=/home/user/reports/reporte1.jpg -name=mbr*
    
    *rep -id=562A -Path=/home/user/reports/report2.pdf -name=disk*
