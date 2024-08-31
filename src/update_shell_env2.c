#include "../includes/minishell.h"

char    ***dup_env_structure(t_env *env, int c, int n, int i)
{
    int b;
    int d;
    char ***dest;

    b = 0;
    d = 0;
    dest = malloc(sizeof(char ***) * (c + n));
    while (b < c)
    {
        if (b != i)
        {
            dest[d] = malloc(sizeof(char **) * 2);
            if (!dest[d])
                return (NULL);
            dest[d][0] = ft_strdup(env->parsed_env[b][0]);
            dest[d][1] = ft_strdup(env->parsed_env[b][1]);
            d++;
        } 
        b++;
    }
    if (i >= 0)
        dest[d] = 0;
    if (n != 'F')
        free_env_var(env->parsed_env);
    return (dest);
}

char    **copy_env_exclude_index(char **src, int c, int n, int i)
{
    int b;
    int d;
    char    **dest;

    d = 0;
    b = 0;
    dest = malloc(sizeof(char **) * (c + n));
    if (!dest)
        return (NULL);
    while (b < c)
    {
        if (b != i)
            dest[d++] = ft_strdup(src[b]);
        b++;
    }
    if (i >= 0)
        dest[d] = 0;
    free_array(src);
    return (dest);
}

void    append_env(char *src, t_env *env)
{
    int c;
    int l;
    int i;
    int b;
    char    *dest;

    c = ft_lentchr(src, '+');
    l = ft_strlen(src) - c - 1;
    dest =  malloc(c + 1);
    ft_strcopy(dest, src, 0, c);
    i = find_var_env(env, dest);
    free(dest);
    if (i >= 0)
    {
        b = ft_strlen(env->env[i]);
        dest = malloc(b + l);
        ft_strcopy(dest, env->env[i], 0, b);
        ft_strcopy(dest + b, src, c + 2, ft_strlen(src));
    }
    else
    replace_env_var(dest, env);
    free(dest);
}