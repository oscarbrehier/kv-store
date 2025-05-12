#ifndef LIBS_H
# define LIBS_H

#include "utils.h"
#include "status_codes.h"

#define RETURN_ON_ERROR(call) do { status = (call); if (status.code != SUCCESS) goto cleanup; } while (0)

void			read_file_into_buffer(const char *filename, char **content);
void			ft_putstr(int fd, const char *str);
void			logger(int fd, char *content, ...);
void			formatTimestamp(time_t timestamp, char *buf, size_t buf_size);
char			*ft_strncpy(char *dest, char *src, unsigned int n);
char			*ft_itoa(int n);
int				ft_strlen(char *str);
int				ft_strcmp(char *s1, char *s2);
int 			count_words(char *str);
int				is_space(char c);
int 			is_valid_key(const char *key);
int             is_quote(char c);
int				intlen(long int nbr);
t_status        construct_table_path(char *filename, char *path, const char **output);


#endif