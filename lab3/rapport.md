# Labo 3 - Miguel Jalube, Michael Gogniat
## Questions
**What is the value of the FC frequency ?**
CPX: GAP8: [LAB3] Frequence = 50000000 [Hz]
**In the CfClient console do you only see your prints ? If not, explain the other prints.**
No i also see other prints. This other prints come from the crazyflie-firmware.
**What part of the data flow is responsible for these ?**
ESP32
**What happens when you connect 2 clients to the GAP8 ? Is it an expected behavior ?**
Impossible de connecter 2 clients. le protocole est fait pour supporter 1 seul client.
**What is the max packet size ?**
-   By printing sizeof(CPXPacket_t) we obtain 1040 B. Which is the max size of a packet.
-   The max size of the datas refering to `uint8_t data[MTU-CPX_HEADER_SIZE];` is :
    ```
    Data Size   = MTU   - CPX_HEADER_SIZE 
                = 1022  - 2 
                = 1020
    ```
**Explain your implementation of the communication protocol.**
1. Imports: Import necessary libraries such as argparse, time, socket, os, struct, numpy, and cv2 (OpenCV).
2. Argument Parsing: Parse command-line arguments to specify the IP address (-n) and port (-p) of the server. There's also an optional argument --save which, when present, indicates that the received images should be saved.
3. Socket Connection: Establish a TCP socket connection with the specified IP address and port.
4. Receiving Data: Continuously receive data from the socket. It first receives information about the image (such as size, resolution, format) and then receives the image data itself.
5. Image Processing: Depending on the format of the received image (indicated in the header), it processes and displays the image using OpenCV. If the format is Bayer (0), it performs a color conversion from BayerBG to BGRA and displays both the raw and color image. If the format is JPEG (1), it decodes and displays the JPEG image.
6. Optional Saving: If the --save argument is provided, the received images are saved to disk.
7. Display: It displays the images using OpenCV's imshow function and waits for a small amount of time before refreshing the display.
8. Performance Metrics: It calculates and prints mean time per image and images per second metrics.
**Provide a plot of one original image and one of the cropped images.**
-
**Explain your cropping algorithm.**
Cet algorithme prend une image en entrée sous forme de tableau unidimensionnel (`input`) ainsi que ses dimensions (`CAM_WIDTH` et `CAM_HEIGHT`). L'objectif est de recadrer cette image pour obtenir une nouvelle image centrée de dimensions prédéfinies (`CROP_WIDTH` par `CROP_HEIGHT`). 

Pour ce faire, la fonction calcule les coordonnées de départ (`x` et `y`) dans l'image d'entrée afin que le recadrage se fasse au centre. Ces coordonnées sont calculées en soustrayant la moitié de la largeur et de la hauteur de l'image recadrée aux dimensions de l'image d'entrée. Cela assure que le recadrage se fasse de manière équilibrée autour du centre de l'image d'origine.

Ensuite, l'algorithme vérifie que les dimensions du recadrage ne débordent pas de l'image d'entrée. Si les coordonnées de départ (`x` et `y`) ou les dimensions de recadrage (`CROP_WIDTH` et `CROP_HEIGHT`) sont incorrectes, l'algorithme affiche un message d'erreur et quitte.

Enfin, l'algorithme copie les pixels de l'image d'entrée qui se trouvent dans la région définie par les coordonnées de départ et les dimensions de recadrage vers une nouvelle image de sortie (`output`). Cette nouvelle image recadrée contient donc uniquement les pixels de l'image d'origine qui se trouvent dans la région délimitée par les coordonnées de départ et les dimensions de recadrage.

**What is the shape of the original image ?**
324x244

## Commentaires sur le laboratoire

Le laboratoire crash lors de la création de la camera task.