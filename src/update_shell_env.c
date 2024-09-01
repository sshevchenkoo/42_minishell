#include "../includes/minishell.h"

void    update_env_value(t_env *env, char *src, int c, int i)
{
    int lk;
    int lv;

    lk = ft_lentchr(src, '=');
    lv = ft_strlen(src) - lk;
    if (i > 0)
    {
        env->parsed_env[c][1] = malloc(lv + 1);
        if (!env->parsed_env[c][1])
            return ;
        ft_strcopy(env->parsed_env[c][1], src, lk + 1, lv + lk);
    }
    else
    {
        env->parsed_env[c][1] = malloc(3);
        if (!env->parsed_env[c][1])
            return ;
        if (!i)
            env->parsed_env[c][1][0] = '\0';
        else
            ft_strcopy(env->parsed_env[c][1], "F1", 0, 2);
    }
}

void    add_env_var(t_env *env, char *src, int l, int i)
{
    int c;
    int d;

    c = 0;
    d = 2;
    if (i)
        d = ft_strlen(src) - l;
    while (env->parsed_env[c])
        c++;
    env->parsed_env = dup_env_structure(env, c, 2, -1);
    env->env = copy_env_exclude_index(env->env, c, 2, -1);
    env->env[c] = malloc(l + d + 1);
    env->parsed_env[c] = malloc(sizeof(char *) * 2);
    env->parsed_env[c][0] = malloc(l + 1);
    if (!env->parsed_env[c][0] || !env->parsed_env[c] || !env->env[c])
        return ;
    ft_strcopy(env->parsed_env[c][0], src, 0, l);
    if (i > 0)
        ft_strcopy(env->env[c], src, 0, ft_strlen(src));
    else
        ft_strcopy(env->env[c], "  ", 0, 1);
    update_env_value(env, src, c, i);
    env->env[c + 1] = 0;
    env->parsed_env[c + 1] = 0;
}

void    remove_env_var(t_env *env, int i)
{
    int l;

    l = 0;
    while (env->parsed_env[l])
        l++;
    if (l > 0)
    {
        env->parsed_env = dup_env_structure(env, l, 0, i);
        env->env = copy_env_exclude_index(env->env, l, 0, i);
    }
}

void    replace_env_var(char *var, t_env *env)
{
    int i;
    int l;
    char    *src;

    l = ft_lentchr(var, '=');
    src = malloc(l + 1);
    if (!src)
        return ;
    ft_strcopy(src, var, 0, l);
    i = find_var_env(env, src);
    if (i >= 0)
        remove_env_var(env, i);
    if (src)
        free(src);
    if (l > 0 && l < ft_strlen(var) - 1)
        add_env_var(env, var, l, 1);
    else if (var[l] == '=')
        add_env_var(env, var, l, 0);
    else if (l == ft_strlen(var))
        add_env_var(env, var, l, -1);
}

void    update_env(t_env *env, int value, char *var)
{
    int a;
    char    *src;
    a = ft_count_digit(value) + ft_strlen(var) + 1;
    src = malloc(a);
    if (!src)
        return ;
    ft_strcopy(src, var, 0, ft_strlen(var));
    src[--a] =  '\0';
    if (!value)
        src[--a] = 0;
    while (value)
    {
        src[--a] = (value % 10) + 48;
        value = value / 10;
    }
    replace_env_var(src, env);
    if (src)
        free(src);
}