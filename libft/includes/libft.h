/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pafroidu <pafroidu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 14:39:49 by diade-so          #+#    #+#             */
/*   Updated: 2025/09/25 12:56:26 by diade-so         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <ctype.h>
# include <fcntl.h>
# include <limits.h>
# include <stddef.h>
# include <string.h>
# include <strings.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <unistd.h>
# include <colors.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

// char
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit(int c);
int		ft_isprint(int c);
bool	ft_isspace(char c);
int		ft_tolower(int c);
int		ft_toupper(int c);

// conv
int		ft_atosize(const char *str, size_t *out);
char	*ft_itoa(int n);
int		ft_safe_atoi(const char *str, int *out);
int		ft_safe_atoll(const char *str, long long *out);

//io
int		ft_putchar_fd(char c, int fd);
int		ft_putendl_fd(char*s, int fd);
void	ft_putnbr_fd(int n, int fd);
int		ft_putstr_fd(char *s, int fd);

// list
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list	*ft_lstnew(void *content);
size_t	ft_lstsize(t_list *lst);

// memory
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);

// print/ft_printf
int		ft_printf(const char *str, ...);

// read
char	*get_next_line(int fd);

// string
void	free_strings_array(char **array);
char	**ft_split(char const *s, char c);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s);
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlcat(char *dest, const char *src, size_t size);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
size_t	ft_strlen(const char *s);
char	*ft_strmapi(char const*s, char (*f)(unsigned int, char));
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strrchr(const char *s, int c);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_substr(char const *s, unsigned int start, size_t len);

#endif
