/*
 *  FRENCH INSTRUCTIONS
 *
 * 1. Copier ce code dans un fichier qu'on appellera 32bitEFI.c
 * 2 . Mettre son ISO de Ubuntu de 16.04 64 bit dans le même dossier.
 * 2. Renommer l'ISO : blackmacbook64.iso
 * 3. Ouvrir un terminal et naviguer dans le dossier en question.
 * 	Si tu ne sais pas comment naviguer le systeme de fichiers dans un terminal, apprends c'est tres simple.
 * 	Tout ce dont tu as besoin, c'est de la commande cd (change directory), pwd (print working directory) et ls (list).
 * 4. Compile dans le terminal le fichier 32bitEFI.c avec la commande : cc -g -Wall -o 32bitEFI 32bitEFI.c
 * 	Tu viens de creer un fichier exécutable qui s'appelle 32bitEFI
 * 4. Pour l'executer tu dois en modifier les droits avec la commande : chmod +x 32bitEFI
 * 5. Maintenant execute le programme avec : ./32bitEFI
 *
 * C'est bon tu viens de "flasher" ton ISO, tu peux maintenant l'utiliser.
 *
 * ENGLISH INSTRUCTIONS
 *
 * 1. Copy the code below in a file called 32bitEFI.c
 * 2. Put the Ubuntu 16.04 .iso file in the same folder.
 * 3. Rename the .iso : blackmacbook64.iso
 * 4. Open a terminal et navigate inn the folder containing both the .c and the .iso
 * 		If you don't know how to navigate files in the terminal, learn it is really straightforward. All you nedd is the cd (change directory),
 * 		the ls (list) commands, and maybe the pwd (print working directory) commands.
 * 5. Compile the 32bitEFI.c file with the following command : cc -g -Wall -o 32bitEFI 32bitEFI.c
 * 		Congrats you just created an executable file called 32bitEFI
 * 6. In order to execute it you must make it an executable with : chmod +x 32bitEFI
 * 7. Now execute 32bitEFI with : ./32bitEFI
 *
 * Congrats you just modified your .iso file so it boots on that old 32bits EFI MacBook
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static char *iso_name = {"blackmacbook64.iso"};

int main(int argc, char **argv)
{
  int fd, ret;
  unsigned char buf[2048 - 64];
  off_t lba;
  size_t buf_size = 2048 - 64;

  fd = open(iso_name, O_RDWR);
  if (fd == -1)
    goto err_ex;
  if (lseek(fd, (off_t) 32768 + 2048 + 71, SEEK_SET) == -1)
    goto err_ex;
  ret = read(fd, buf, 4);
  if (ret == -1)
    goto err_ex;
  if (ret < 4) {
    fprintf(stderr, "Cannot read 4 bytes from %s\n", iso_name);
    exit(1);
  }
  lba = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
  if (lseek(fd, lba * 2048 + 64, SEEK_SET) == -1)
    goto err_ex;
  memset(buf, 0, buf_size);
  ret = write(fd, buf, buf_size);
  if (ret == -1)
    goto err_ex;
  if (ret < buf_size) {
    fprintf(stderr, "Cannot write %d bytes to %s\n", (int) buf_size, iso_name);
    exit(1);
  }
  close(fd);
  printf("done\n");
  exit(0);
err_ex:;
  perror(iso_name);
  exit(1);
}
