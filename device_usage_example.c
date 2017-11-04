// make
// Bruno Produit

#include "unistd.h"
#include "stdio.h"
#include "fcntl.h"

int main(void)
{
    int           fd;
    int           nbByte;
    int           var1;
    int           var2;

    var1 = 3;
    fd = open ("/dev/my_device", O_RDWR);
    printf ("File descriptor = %d\n", fd);
    
    if (fd!=-1)
    {
        nbByte=write(fd, &var1, sizeof(var1));
        printf ("write, nbByte=%d\n", nbByte);
        
        nbByte=read(fd, &var2, sizeof(var2));
        printf ("read, nbByte=%d, var2=%d\n", nbByte, var2);
        
        close (fd);
    }    
    return 0;
}
