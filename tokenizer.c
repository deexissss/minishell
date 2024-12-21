#include "minishell.h"

char	*ft_strtok(char *str, const char *delimiters)
{
    static char	*stock = NULL;
    char		*ptr;

    if (str != NULL)
        stock = str;
    if (stock == NULL)
        return NULL;
    // Skip initial delimiters
    while (*stock != '\0' && ftstrchr(delimiters, *stock) != NULL)
        stock++;
    if (*stock == '\0')
        return NULL;
    ptr = stock;
    // Find the end of the token
    while (*stock != '\0' && ftstrchr(delimiters, *stock) == NULL)
        stock++;
    if (*stock != '\0')
    {
        *stock = '\0';
        stock++;
    }
    return ptr;
}