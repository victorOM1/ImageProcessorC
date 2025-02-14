#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Prototipos de funciones que manejan operaciones sobre imágenes
void verificar (FILE *arch); // Verifica si un archivo pudo abrirse correctamente.
void caracteristicas (char nombre[], unsigned int *ancho, unsigned int *alto); // Extrae características de un archivo BMP o PNM.
void SepCanal(char nombre[], int ancho, int alto, unsigned short r, unsigned short g, unsigned short b); // Genera una imagen con solo uno de los canales RGB.
void escalaDG (char nombre[], int ancho, int alto); // Convierte una imagen a escala de grises.
void BlancoYNegro (char nombre[], int ancho, int alto, int umbral); // Convierte una imagen a blanco y negro según un umbral.
void Histograma(char nombre[], int ancho, int alto); // Genera histogramas para los canales de color o escala de grises.
void Mezcla (char nombre1[],char nombre2[], int alpha); // Mezcla dos imágenes aplicando un factor alpha.
void ImpHelp(); // Imprime las instrucciones de uso del programa.

int main(int argc, char *argv[]) {
    char nombre1[256], nombre2[256]; // Almacenan los nombres de los archivos de entrada.
    unsigned int ancho, alto; // Dimensiones de las imágenes.
    FILE *archivo1, *archivo2; // Punteros a archivos para las imágenes.

    // Manejo de argumentos para seleccionar una operación específica
    if (argv[1][0] == '1') {
        // Separar los canales RGB de una imagen
        strcpy(nombre1, argv[2]); 
        caracteristicas(nombre1, &ancho, &alto); // Obtiene las dimensiones de la imagen.
        SepCanal(nombre1, (int)ancho, (int)alto, 1, 0, 0); // Canal rojo
        SepCanal(nombre1, (int)ancho, (int)alto, 0, 1, 0); // Canal verde
        SepCanal(nombre1, (int)ancho, (int)alto, 0, 0, 1); // Canal azul

    } else if (argv[1][0] == '2') {
        // Separar solo el canal rojo
        strcpy(nombre1, argv[2]);
        caracteristicas(nombre1, &ancho, &alto);
        SepCanal(nombre1, (int)ancho, (int)alto, 1, 0, 0);

    } else if (argv[1][0] == '3') {
        // Separar solo el canal verde
        strcpy(nombre1, argv[2]);
        caracteristicas(nombre1, &ancho, &alto);
        SepCanal(nombre1, (int)ancho, (int)alto, 0, 1, 0);

    } else if (argv[1][0] == '4') { // Opción 4: Separar canal azul
        strcpy(nombre1, argv[2]);
        caracteristicas(nombre1, &ancho, &alto);
        SepCanal(nombre1, (int)ancho, (int)alto, 0, 0, 1);

    } else if (argv[1][0] == '5') { // Opción 5: Convertir a escala de grises
        strcpy(nombre1, argv[2]);
        caracteristicas(nombre1, &ancho, &alto);
        escalaDG(nombre1, (int)ancho, (int)alto);

    } else if (argv[1][0] == '6') { // Opción 6: Convertir a blanco y negro
        strcpy(nombre1, argv[3]);
        caracteristicas(nombre1, &ancho, &alto);
        BlancoYNegro(nombre1, (int)ancho, (int)alto, atoi(argv[2])); // Umbral como argumento
        
    } else if (argv[1][0] == '7') { // Opción 7: Generar histograma
        strcpy(nombre1, argv[2]);
        caracteristicas(nombre1, &ancho, &alto);
        Histograma(nombre1, (int)ancho, (int)alto);
        
    } else if (argv[1][0] == '8') { // Opción 8: Mezclar dos imágenes
        strcpy(nombre1, argv[3]);
        strcpy(nombre2, argv[4]);
        caracteristicas(nombre1, &ancho, &alto);
        caracteristicas(nombre2, &ancho, &alto);
        Mezcla(nombre1, nombre2, atoi(argv[2])); // Alpha como argumento

    } else if (argv[1][0] == '9') { // Opción 9: Aplicar todas las operaciones
        strcpy(nombre1, argv[4]);
        strcpy(nombre2, argv[5]);
        caracteristicas(nombre1, &ancho, &alto);
        caracteristicas(nombre2, &ancho, &alto);
        SepCanal(nombre1, (int)ancho, (int)alto, 1, 0, 0);
        SepCanal(nombre1, (int)ancho, (int)alto, 0, 1, 0);
        SepCanal(nombre1, (int)ancho, (int)alto, 0, 0, 1);
        SepCanal(nombre2, (int)ancho, (int)alto, 1, 0, 0);
        SepCanal(nombre2, (int)ancho, (int)alto, 0, 1, 0);
        SepCanal(nombre2, (int)ancho, (int)alto, 0, 0, 1);
        escalaDG(nombre1, (int)ancho, (int)alto);
        escalaDG(nombre2, (int)ancho, (int)alto);
        BlancoYNegro(nombre1, (int)ancho, (int)alto, atoi(argv[2]));
        BlancoYNegro(nombre2, (int)ancho, (int)alto, atoi(argv[2]));
        Histograma(nombre1, (int)ancho, (int)alto);
        Histograma(nombre2, (int)ancho, (int)alto);
        Mezcla(nombre1, nombre2, atoi(argv[3]));
    } else if (argv[1][0] == 'h' || argv[1][0] == 'H'||
        (argv[1][0] == '-'&& (argv[1][1] == 'h' || argv[1][1] == 'H'))) { // Opción de ayuda
        ImpHelp();
    } else {
        printf("Error %s no se reconoce\n", argv[1]); // Manejo de errores
    }

    return 0;
}
// Si el archivo no pudo abrirse, se muestra un mensaje de error y el programa se cierra con un código de salida 3.
void verificar (FILE *arch) {
    if (arch == NULL) {
        printf("No se pudo abrir el archivo\n");
        exit(3);
    }
}

void caracteristicas (char nombre[], unsigned int *ancho, unsigned int *alto) {
    int bitsPC = 0; // Cantidad de bits por canal de color
    unsigned int tamano, maxColor; // Tamaño del archivo y valor máximo de color.
    unsigned short bitsPP, bits; // Bits por pixel en BMP y total por píxel.
    char formato[3]; // Almacena el formato del archivo (BMP o PNM).
    FILE *foto;

    // Abre el archivo en modo binario y verifica su estado
    foto = fopen(nombre, "rb");
    verificar(foto);
    fscanf(foto, "%2s", formato); // Lee los primeros dos caracteres del archivo (identifica el formato)
    fclose(foto);

    if (formato[1] == 'M') { // Si el archivo es un BMP

        printf("\n%s es un archivo BMP\n", nombre);
        foto = fopen(nombre, "rb");
        verificar(foto);

        // Leer el tamaño del archivo (bytes 2-5)
        fseek(foto, 2, SEEK_SET);
        fread(&tamano, 4, 1, foto);

        // Leer las dimensiones de la imagen (bytes 18-25)
        fseek(foto, 18, SEEK_SET);
        fread(ancho, 4, 1, foto);
        fread(alto, 4, 1, foto);

        // Leer los bits por píxel (bytes 28-29)
        fseek(foto, 28, SEEK_SET);
        fread(&bits, 2, 1, foto);

        printf("Tamaño del archivo: %d bytes\n", tamano);
        printf("Ancho: %d\n", *ancho);
        printf("Alto: %d\n", *alto);
        printf("Bits por pixel: %d\n", bits);

        fclose(foto);

    } else if (formato[0] == 'P') { // Si el archivo es un PNM (P3 o P6)
        printf("\n%s es un archivo PNM (%s)\n", nombre, formato);

        foto = fopen(nombre, "r");
        verificar(foto);

        // Leer ancho, alto y el máximo valor de color
        fseek(foto, 2, SEEK_SET);
        fscanf(foto, "%u %u %u", ancho, alto, &maxColor);

        // Calcular el tamaño en bytes basado en las dimensiones y profundidad de color
        tamano = *ancho * *alto * 3;
        if (maxColor <= 255) { // Determinar bits por canal
            bitsPC = 8; 
        } else {
            bitsPC = 16;
        }

        bitsPP = bitsPC * 3; // Bits por pixel
        printf("Tamaño del archivo: %d bytes\n", tamano);
        printf("Ancho: %d\n", *ancho);
        printf("Alto: %d\n", *alto);
        printf("Bits por pixel: %d\n", bitsPP);

        fclose(foto);

    } else {
        printf("\nNo se reconoce el archivo\n");
    }
}
void SepCanal(char nombre[], int ancho, int alto, unsigned short r, unsigned short g, unsigned short b) {
    char nuevoNombre[256], nombreT[230], id[4]="", formato[3];
    int fAlmohadilla, i, j;
    unsigned char datos[3], rojo, verde, azul;
    FILE *archivoO, *archivoN;

    // Abrir archivo original en modo binario
    strcpy(nombreT, nombre);
    archivoO = fopen(nombreT, "rb");
    verificar(archivoO);

    // Crear un nuevo nombre de archivo con el sufijo de canal (R, G o B)
    nombreT[strlen(nombreT) - 4] = '\0'; // Remover extensión original
    if (r == 1) {
        strcat(id, "R");
    }
    if (g == 1) {
        strcat(id, "G");
    }
    if (b == 1) {
        strcat(id, "B");
    }
    strcat(nombreT, "_");
    strcat(nombreT, id);
    strcat(nombreT, ".pnm");
    strcpy(nuevoNombre, nombreT);

    // Crear nuevo archivo para guardar el canal separado
    archivoN = fopen(nuevoNombre, "wb");
    verificar(archivoN);

    fscanf(archivoO, "%2s", formato); // Leer el formato del archivo original (BMP o PNM)

    // Ajustar el punto de inicio de datos según el formato
    if (formato[1] == 'M') {
        fseek(archivoO, 54, SEEK_SET); // BMP: Saltar cabecera
    } else {
        fseek(archivoO, 5, SEEK_SET); // PNM: Saltar cabecera
    }

    // Calcular bytes por fila para BMP (alineación a 4 bytes)
    fAlmohadilla = (ancho * 3 + 3);
    fAlmohadilla = fAlmohadilla - fAlmohadilla % 4;

    // Escribir encabezado del archivo PNM en el nuevo archivo
    fprintf(archivoN, "P6\n%d %d\n255\n", ancho, alto);
    // Procesar datos según el formato del archivo original
    if (formato[1] == 'M') { // BMP
        for (i = alto - 1; i >= 0; i--) { // Leer fila invertida (BMP)
            fseek(archivoO, 54 + i * fAlmohadilla, SEEK_SET);
            for (j = 0; j < ancho; j++) {
                fread(datos, sizeof(unsigned char), 3, archivoO); // Escoger el canal (multiplicar por 0 o 1 según r, g, b)
                datos[0] = datos[0]*b; //Azul
                datos[1] = datos[1]*g; //Verde
                datos[2] = datos[2]*r; //Rojo
                // Guardar en el nuevo archivo
                fwrite(&datos[2], sizeof(unsigned char), 1, archivoN);
                fwrite(&datos[1], sizeof(unsigned char), 1, archivoN);
                fwrite(&datos[0], sizeof(unsigned char), 1, archivoN);
            }
        }
    } else if (formato[1] == '6') { // P6 (binario)
        for (i = alto - 1; i >= 0; i--) {
            for (j = 0; j < ancho; j++) {
                fread(datos, sizeof(unsigned char), 3, archivoO);
                datos[0] = datos[0]*r;
                datos[1] = datos[1]*g;
                datos[2] = datos[2]*b;
                fwrite(datos, sizeof(unsigned char), 3, archivoN);
            }
        }
    }else if (formato[1] == '3') { // P3 (ASCII)
        for (i = 0; i < ancho * alto; i++) {
            fscanf(archivoO, "%hhu %hhu %hhu", &datos[0], &datos[1], &datos[2]);
            datos[0] = datos[0]*r;
            datos[1] = datos[1]*g;
            datos[2] = datos[2]*b;
            fwrite(datos, 1, 3, archivoN);
        }
    }

    fclose(archivoO);
    fclose(archivoN);

    printf("Nuevo archivo creado: %s\n", nuevoNombre);
}

void escalaDG (char nombre[], int ancho, int alto) {
    char nuevoNombre[256], nombreT[230], formato[3];
    int fAlmohadilla, i, j, pixel;
    unsigned char datos[3], rojo, verde, azul;
    FILE *archivoO, *archivoN;

    // Abrir el archivo original en modo binario
    strcpy(nombreT, nombre);
    archivoO = fopen(nombreT, "rb");
    verificar(archivoO);

    // Crear un nuevo nombre para el archivo de salida
    nombreT[strlen(nombreT) - 4] = '\0'; // Eliminar la extensión original
    strcat(nombreT, "_EDG.pnm"); // Agregar sufijo para escala de grises
    strcpy(nuevoNombre, nombreT);

    // Abrir el archivo de salida para escribir
    archivoN = fopen(nuevoNombre, "wb");
    verificar(archivoN);

    fscanf(archivoO, "%2s", formato); // Leer el formato del archivo original (BMP o PNM)

    // Ajustar el punto de inicio de los datos según el formato
    if (formato[1] == 'M') {
        fseek(archivoO, 54, SEEK_SET); // BMP: Saltar cabecera
    } else {
        fseek(archivoO, 5, SEEK_SET); // PNM: Saltar cabecera
    } 

    // Calcular bytes por fila para BMP (alineación a 4 bytes)
    fAlmohadilla = (ancho * 3 + 3);
    fAlmohadilla = fAlmohadilla - fAlmohadilla % 4;

    // Escribir encabezado del archivo PNM de salida
    fprintf(archivoN, "P5\n%d %d\n255\n", ancho, alto);
    // Procesar los datos según el formato original
    if (formato[1] == 'M') { // BMP
        for (i = alto - 1; i >= 0; i--) {
            fseek(archivoO, 54 + i * fAlmohadilla, SEEK_SET); // Leer fila invertida (BMP)
            for (j = 0; j < ancho; j++) {
                fread(datos, sizeof(unsigned char), 3, archivoO);
                // Convertir a escala de grises calculando el promedio
                pixel = (datos[0] + datos[1] + datos[2]) / 3;
                fwrite(&pixel, sizeof(unsigned char), 1, archivoN);
            }
        }
    } else if (formato[1] == '6') { // P6 (binario)
        for (i = alto - 1; i >= 0; i--) {
            for (j = 0; j < ancho; j++) {
                fread(datos, sizeof(unsigned char), 3, archivoO);
                pixel = (datos[0] + datos[1] + datos[2]) / 3;
                fwrite(&pixel, sizeof(unsigned char), 1, archivoN);
            }
        }
    }else if (formato[1] == '3') { // P3 (ASCII)
        for (i = 0; i < ancho * alto; i++) {
            fscanf(archivoO, "%hhu %hhu %hhu", &rojo, &verde, &azul);
            pixel = (rojo + verde + azul) / 3;
            fwrite(&pixel, 1, 1, archivoN);
        }
    }

    fclose(archivoO);
    fclose(archivoN);

    printf("Nuevo archivo creado: %s\n", nuevoNombre);
}

void BlancoYNegro (char nombre[], int ancho, int alto, int umbral) {
    char nuevoNombre[256], nombreT[230], formato[3];
    int fAlmohadilla, i, j, pixel;
    unsigned char datos[3], rojo, verde, azul;
    FILE *archivoO, *archivoN;
    
    // Validar que el umbral esté dentro del rango válido
    if (umbral < 0 || umbral > 255) {
        printf("El valor de alpha debe estar entre 0 y 255\n");
        exit(4);
    }
    // Abrir el archivo original en modo binario
    strcpy(nombreT, nombre);
    archivoO = fopen(nombreT, "rb");
    verificar(archivoO);
    // Crear un nuevo nombre para el archivo de salida
    nombreT[strlen(nombreT) - 4] = '\0'; // Eliminar la extensión original
    strcat(nombreT, "_BN.pnm"); // Agregar sufijo para blanco y negro
    strcpy(nuevoNombre, nombreT);

    archivoN = fopen(nuevoNombre, "wb"); // Abrir el archivo de salida para escribi
    verificar(archivoN);

    fscanf(archivoO, "%2s", formato); // Leer el formato del archivo original (BMP o PNM)

    // Ajustar el punto de inicio de los datos según el formato
    if (formato[1] == 'M') {
        fseek(archivoO, 54, SEEK_SET); // BMP: Saltar cabecera
    } else {
        fseek(archivoO, 5, SEEK_SET); // PNM: Saltar cabecera
    }

    fAlmohadilla = (ancho * 3 + 3); // Calcular bytes por fila para BMP (alineación a 4 bytes)
    fAlmohadilla = fAlmohadilla - fAlmohadilla % 4;

    // Escribir encabezado del archivo PNM de salida
    fprintf(archivoN, "P5\n%d %d\n255\n", ancho, alto);
    // Procesar los datos según el formato original
    if (formato[1] == 'M') { // BMP
        for (i = alto - 1; i >= 0; i--) {
            fseek(archivoO, 54 + i * fAlmohadilla, SEEK_SET); // Leer fila invertida (BMP)
            for (j = 0; j < ancho; j++) {
                fread(datos, sizeof(unsigned char), 3, archivoO);
                // Calcular el valor en escala de grises
                pixel = (datos[0] + datos[1] + datos[2]) / 3;
                // Aplicar umbral: blanco (255) o negro (0)
                if (pixel > (int)umbral) {
                    pixel = 255;
                } else {
                    pixel = 0;
                }
                fwrite(&pixel, sizeof(unsigned char), 1, archivoN);
            }
        }
    } else if (formato[1] == '6') { // P6 (binario)
        for (i = alto - 1; i >= 0; i--) {
            for (j = 0; j < ancho; j++) {
                fread(datos, sizeof(unsigned char), 3, archivoO);
                pixel = (datos[0] + datos[1] + datos[2]) / 3;
                if (pixel > (int)umbral) {
                    pixel = 255;
                } else {
                    pixel = 0;
                }
                fwrite(&pixel, sizeof(unsigned char), 1, archivoN);
            }
        }
    }else if (formato[1] == '3') { // P3 (ASCII)
        for (i = 0; i < ancho * alto; i++) {
            fscanf(archivoO, "%hhu %hhu %hhu", &rojo, &verde, &azul);
            pixel = (rojo + verde + azul) / 3;
            if (pixel > (int)umbral) {
                pixel = 255;
            } else {
                pixel = 0;
            }
            fwrite(&pixel, 1, 1, archivoN);
        }
    }

    fclose(archivoO);
    fclose(archivoN);

    printf("Nuevo archivo creado: %s\n", nuevoNombre);
}

void Histograma(char nombre[], int ancho, int alto) {
    char nuevoNombre[256]="", nombreT[230], formato[3];
    int fAlmohadilla, i, j, bits;
    long int CR[256] = {0}, CG[256] = {0}, CB[256] = {0};
    unsigned char datos[3], rojo, verde, azul, dato[1];
    FILE *archivoO, *archivoR, *archivoG, *archivoB;

    // Abrir el archivo original en modo binario
    strcpy(nombreT, nombre);
    archivoO = fopen(nombreT, "rb");
    verificar(archivoO);

    // Leer el formato del archivo
    nombreT[strlen(nombreT) - 4] = '\0';
    fscanf(archivoO, "%2s", formato);
    // Ajustar el punto de inicio de datos según el formato
    if (formato[1] == 'M') {
        fseek(archivoO, 2, SEEK_SET);

        fseek(archivoO, 18, SEEK_SET);

        fseek(archivoO, 28, SEEK_SET);
        fread(&bits, 2, 2, archivoO);
        fseek(archivoO, 54, SEEK_SET);
    } else {
        fseek(archivoO, 5, SEEK_SET);
    }

    fAlmohadilla = (ancho * 3 + 3); // Calcular bytes por fila para BMP (alineación a 4 bytes)
    fAlmohadilla = fAlmohadilla - fAlmohadilla % 4;

    // Abrir archivos de salida para cada canal (R, G, B)
    if (formato[1] == '6' || formato[1] == '3' || bits == 24) {
        strcpy(nuevoNombre, nombreT);
        strcat(nuevoNombre, "_histR.txt");
        archivoR = fopen(nuevoNombre, "wb");
        verificar(archivoR);
        printf("Nuevo archivo creado: %s\n", nuevoNombre);

        strcpy(nuevoNombre, nombreT);
        strcat(nuevoNombre, "_histG.txt");
        archivoG = fopen(nuevoNombre, "wb");
        verificar(archivoG);
        printf("Nuevo archivo creado: %s\n", nuevoNombre);

        strcpy(nuevoNombre, nombreT);
        strcat(nuevoNombre, "_histB.txt");
        archivoB = fopen(nuevoNombre, "wb");
        verificar(archivoB);
        printf("Nuevo archivo creado: %s\n", nuevoNombre);

        // Procesar los datos y calcular las frecuencias para cada canal
        if (formato[1] == 'M') { // BMP
            for (i = alto - 1; i >= 0; i--) {
                fseek(archivoO, 54 + i * fAlmohadilla, SEEK_SET);
                for (j = 0; j < ancho; j++) {
                    fread(datos, sizeof(unsigned char), 3, archivoO);
                    CR[datos[2]]++; // Incrementar frecuencia del canal rojo
                    CG[datos[1]]++; // Incrementar frecuencia del canal verde
                    CB[datos[0]]++; // Incrementar frecuencia del canal azul
                }
            }
        } else if (formato[1] == '6') { // P6 (binario)
            for (i = alto - 1; i >= 0; i--) {
                for (j = 0; j < ancho; j++) {
                    fread(datos, sizeof(unsigned char), 3, archivoO);
                    CR[datos[0]]++;
                    CG[datos[1]]++;
                    CB[datos[2]]++;
                }
            }
        }else if (formato[1] == '3') { // P3 (ASCII)
            for (i = 0; i < ancho * alto; i++) {
                fscanf(archivoO, "%hhu %hhu %hhu", &rojo, &verde, &azul);
                CR[rojo]++;
                CG[verde]++;
                CB[azul]++;
            }
        }
        // Escribir frecuencias en los archivos de salida
        fwrite("Valor\tFrecuencia\tHistograma\n", 1, 28, archivoR);
        fwrite("Valor\tFrecuencia\tHistograma\n", 1, 28, archivoG);
        fwrite("Valor\tFrecuencia\tHistograma\n", 1, 28, archivoB);
        for (i = 0; i < 256; i++) {
            fprintf(archivoR, "%d\t%ld\t\t", i, CR[i]);
            for (j = 0; j < CR[i]; j++) {
                fwrite("*", 1, 1, archivoR);
            }
            fwrite("\n", 1, 1, archivoR);

            fprintf(archivoG, "%d\t%ld\t\t", i, CG[i]);
            for (j = 0; j < CG[i]; j++) {
                fwrite("*", 1, 1, archivoG);
            }
            fwrite("\n", 1, 1, archivoG);

            fprintf(archivoB, "%d\t%ld\t\t", i, CB[i]);
            for (j = 0; j < CB[i]; j++) {
                fwrite("*", 1, 1, archivoB);
            }
            fwrite("\n", 1, 1, archivoB);
        }
        fclose(archivoG);
        fclose(archivoB);
    } else {
        strcat(nombreT, "_histGris.txt");
        strcpy(nuevoNombre, nombreT);

        archivoR = fopen(nuevoNombre, "wb");
        verificar(archivoR);
        printf("Nuevo archivo creado: %s\n", nuevoNombre);

        if (formato[1] == 'M') {
            for (i = alto - 1; i >= 0; i--) {
                fseek(archivoO, 54 + i * fAlmohadilla, SEEK_SET);
                for (j = 0; j < ancho; j++) {
                    fread(dato, 1, 1, archivoO);
                    CR[dato[0]]++;
                }
            }
        } else if (formato[1] == '5') {
            for (i = alto - 1; i >= 0; i--) {
                for (j = 0; j < ancho; j++) {
                    fread(dato, 1, 1, archivoO);
                    CR[dato[0]]++;
                }
            }
        }else if (formato[1] == '2') {
            for (i = 0; i < ancho * alto; i++) {
                fscanf(archivoO, "%hhu", &rojo);
                CR[rojo]++;
            }
        }
    }

    fwrite("Valor\tFrecuencia\tHistograma\n", 1, 28, archivoR);
    for (i = 0; i < 256; i++) {
        fprintf(archivoR, "%d\t%ld\t\t", i, CR[i]);
        for (j = 0; j < CR[i]; j++) {
            fwrite("*", 1, 1, archivoR);
        }
        fwrite("\n", 1, 1, archivoR);
    }

    fclose(archivoO);
    fclose(archivoR);
}

void Mezcla (char nombre1[],char nombre2[], int alpha) {
    char nuevoNombre[256], formato1[3], formato2[3];
    int fAlmohadilla, i, j;
    unsigned int ancho1, alto1, ancho2, alto2;
    unsigned char datos1[3]={0}, datos2[3]={0}, pixel[3], dt;
    FILE *archivo1, *archivo2, *archivoN;

    // Validar que alpha esté en el rango 0-255
    if (alpha < 0 || alpha > 255) {
        printf("El valor de alpha debe estar entre 0 y 255\n");
        exit(5);
    }

    // Abrir los archivos originales
    archivo1 = fopen(nombre1, "rb");
    verificar(archivo1);

    archivo2 = fopen(nombre2, "rb");
    verificar(archivo2);

    // Crear un nuevo nombre para el archivo de salida
    nombre1[strlen(nombre1) - 4] = '\0';
    nombre2[strlen(nombre2) - 4] = '\0';

    strcat(nombre1, "_");
    strcat(nombre1, nombre2);
    strcat(nombre1, ".pnm");
    strcpy(nuevoNombre, nombre1);
     // Abrir el archivo de salida
    archivoN = fopen(nuevoNombre, "wb");
    verificar(archivoN);

    fscanf(archivo1, "%2s", formato1); // Leer dimensiones y formato del primer archivo

    if (formato1[1] == 'M') {
        fseek(archivo1, 18, SEEK_SET);
        fread(&ancho1, 4, 1, archivo1);
        fread(&alto1, 4, 1, archivo1);
        fseek(archivo1, 54, SEEK_SET); // Saltar cabecera
    } else {
        fseek(archivo1, 2, SEEK_SET);
        fscanf(archivo1, "%u %u", &ancho1, &alto1);
        fseek(archivo1, 15, SEEK_SET); // Saltar cabecera
    }

    fscanf(archivo2, "%2s", formato2); // Leer dimensiones y formato del segundo archivo

    if (formato2[1] == 'M') {
        fseek(archivo2, 18, SEEK_SET);
        fread(&ancho2, 4, 1, archivo2);
        fread(&alto2, 4, 1, archivo2);
        fseek(archivo2, 54, SEEK_SET); // Saltar cabecera
    } else {
        fseek(archivo2, 2, SEEK_SET);
        fscanf(archivo2, "%u %u", &ancho2, &alto2);
        fseek(archivo2, 15, SEEK_SET); // Saltar cabecera
    }
    // Validar que las dimensiones sean iguales
    if (ancho1 != ancho2 || alto1 != alto2) {
        printf("Las imagenes no tienen las mismas dimensiones\n");
        exit(4);
    }
    // Calcular bytes por fila para BMP (alineación a 4 bytes)
    fAlmohadilla = (ancho1 * 3 + 3);
    fAlmohadilla = fAlmohadilla - fAlmohadilla % 4;

    fprintf(archivoN, "P6\n%d %d\n255\n", ancho1, alto1); // Escribir encabezado del archivo de salida

    // Procesar los datos y realizar la mezcla
    for (i = alto1 - 1; i >= 0; i--) {
        if (formato1[1] == 'M') {
            fseek(archivo1, 54 + i * fAlmohadilla, SEEK_SET);
        }
        if (formato2[1] == 'M') {
            fseek(archivo2, 54 + i * fAlmohadilla, SEEK_SET);
        }
        for (j = 0; j < ancho1; j++) { // Leer píxeles de las imágenes
            if (formato1[1] == '6' || formato1[1] == 'M') {
                fread(datos1, sizeof(unsigned char), 3, archivo1);
                if (formato1[1] == 'M') {
                    dt = datos1[0];
                    datos1[0] = datos1[2];
                    datos1[2] = dt;
                }
            } else if (formato1[1] == '3') {
                fscanf(archivo1, "%hhu %hhu %hhu", &datos1[0], &datos1[1], &datos1[2]);
            }
            if (formato2[1] == '6' || formato2[1] == 'M') {
                fread(datos2, sizeof(unsigned char), 3, archivo2);
                if (formato2[1] == 'M') {
                    dt = datos2[0];
                    datos2[0] = datos2[2];
                    datos2[2] = dt;
                }
            } else if (formato2[1] == '3') {
                fscanf(archivo2, "%hhu %hhu %hhu", &datos2[0], &datos2[1], &datos2[2]);
            }
                // Mezclar los píxeles
                pixel[0] = (datos1[0] * alpha) / 256 + (datos2[0] * (255 - alpha)) / 256;
                pixel[1] = (datos1[1] * alpha) / 256 + (datos2[1] * (255 - alpha)) / 256;
                pixel[2] = (datos1[2] * alpha) / 256 + (datos2[2] * (255 - alpha)) / 256;

            fwrite(&pixel, sizeof(unsigned char), 3, archivoN); // Escribir píxel mezclado en el archivo de salida
        }
    }


    fclose(archivo1);
    fclose(archivo2);
    fclose(archivoN);

    printf("Nuevo archivo creado: %s\n", nuevoNombre);
}
void ImpHelp(){ //Esta función imprime las instrucciones de uso del programa, mostrando las diferentes opciones disponibles para el usuario, cómo usarlas, y qué parámetros deben proporcionarse.
    printf("./main _  _                _\n");
    printf("       ^  ^                ^\n");
    printf("  opcion  tipo de archivo  parametros\n");
    printf("Opciones:\n");
    printf("1. Separar canales de una imagen\n");
    printf("\tEjemplo: ./main 1p/b imagen.bmp/pnm\n");
    printf("2. Separar canal rojo de una imagen\n");
    printf("\tEjemplo: ./main 2p/b imagen.bmp/pnm\n");
    printf("3. Separar canal verde de una imagen\n");
    printf("\tEjemplo: ./main 3p/b imagen.bmp/pnm\n");
    printf("4. Separar canal azul de una imagen\n");
    printf("\tEjemplo: ./main 4p/b imagen.bmp/pnm\n");
    printf("5. Convertir a escala de grises\n");
    printf("\tEjemplo: ./main 5p/b imagen.bmp/pnm\n");
    printf("6. Convertir a blanco y negro\n");
    printf("\tEjemplo: ./main 6p/b umbral imagen.bmp/pnm\n");
    printf("7. Generar histograma\n");
    printf("\tEjemplo: ./main 7p/b imagen.bmp/pnm\n");
    printf("8. Mezclar dos imagenes\n");
    printf("\tEjemplo: ./main 8p/b alpha imagen1.bmp/pnm imagen2.bmp/pnm\n");
    printf("9. Aplicar todas las funciones a dos imagenes\n");
    printf("\tEjemplo: ./main 9p/b umbral alpha imagen1.bmp/pnm imagen2.bmp/pnm\n");
}
