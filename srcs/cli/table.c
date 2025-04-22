#include "cli.h"
#include "kv_store.h"
#include "libs.h"

void    init_data_table(t_data_table *data_table, const char *title, const char *column_titles[])
{
    int i;

	i = 0;
	memset(data_table, 0, sizeof(t_data_table));
	if (!(set_title(data_table, title)))
        return ;
    // data_table->data = malloc(sizeof(char **) * 10); // Allocate memory for data
    // if (!data_table->data) {
    //      logger(1, ERROR_MEMORY_ALLOCATION);
    //     return;
    // }
    // for (i = 0; i < 10; i++) {
    //     data_table->data[i] = NULL;
    // }
	while (column_titles[i])
	{
		add_column(data_table, column_titles[i]);
		i++;
	}
}

int	set_title(t_data_table *table, const char *title)
{
	if (!table) 
		return (0);
	if (table->title)
		free(table->title);
    table->title = strdup(title);
	return (1);
}

void    add_column(t_data_table *table, const char *title)
{
    int i;

    i = table->column_count != 0 ? table->column_count : 0;
    strncpy(table->columns[i].title, title, sizeof(table->columns[i].title) - 1);
    table->columns[i].title[sizeof(table->columns[i].title) - 1] = '\0';
    table->columns[i].width = DEF_COL_WIDTH;
    table->column_count++;
}

void	cleanup_data_table(char ***data, int row_count)
{
    int i;
    int j;

    if (!data)
        return ;
    if (row_count < 0)
    {
        row_count = 0;
        while (data[row_count] != NULL)
        row_count++;
    }
    i = 0;
    while (i < row_count)
    {
        if (data[i])
        {
            j = 0;
            while (data[i][j] != NULL)
            {
                free(data[i][j]);
                j++;
            }
            free(data[i]);
        }
        i++;
    }
    if (data[row_count])
        free(data[row_count]);
    free(data);
}

void    display_title(t_data_table table)
{
    int i;
    int width;
    int padding;

    i = 0;
    width = 0;
    while (i < table.column_count)
    {
        width += table.columns[i].width + 3;
        i++;
    }
    padding = (width - ft_strlen(table.title)) / 2;
    printf("\n  %*s%s\n", padding, "", table.title);
}

void    display_header(t_data_table table)
{   
    int i;
    int j;

    i = 0;
    printf("\n");
    while (i < table.column_count)
    {
        if (i == 0)
            printf("  %-*s   ", table.columns[i].width, table.columns[i].title);
        else
            printf(" %-*s   ", table.columns[i].width, table.columns[i].title);
        if (i < table.column_count - 1)
            printf("|");
        i++;
    }
    printf("\n");
    printf("  ");
    i = 0;
    j = 0;
    while (i < table.column_count)
    {
        j = 0;
        while (j < table.columns[i].width + (i == 0 ? 2 : 1))
        {
            printf("-");
            j++;
        }
        if (i < table.column_count - 1)
            printf("-+-");
        i++;
    }
    printf("\n");
}

void    display_footer(int  rows)
{
    printf("  (%d %s)\n\n", rows, rows > 1 ? "rows" : "row");
}

int display_data(t_data_table table)
{
    int row;
    int col;

    row = 0;
    while (table.data[row])
    {
        col = 0;
        while (col < table.column_count)
        {
            if (col == 0)
                printf("  %-*s   ", table.columns[col].width, table.data[row][col]);
            else
                printf(" %-*s   ", table.columns[col].width, table.data[row][col]);
            if (col < table.column_count - 1)
                printf("|");
            col++;
        }
        printf("\n");
        row++;
    }
    return (row);
}

void    display_table(t_data_table table)
{
    int             i;
    int             j;
    int             rows;
    int             tmp;
    // const char      *columns_title[] = {"key", "value", "test", NULL};

    // i = 0;
    // while (columns_title[i])
    // {
    //     strncpy(table.columns[i].title, columns_title[i], sizeof(table.columns[i].title));
    //     table.columns[i].title[sizeof(table.columns[i].title) + 1] = '\0';
    //     table.columns[i].width = DEF_COL_WIDTH;
    //     i++;
    // }
    j = 0;
    i = 0;
    table.column_count = 0;
    while (table.data[i][j])
    {
        table.column_count++;
        j++;
    }
    while (table.data[i])
    {
        j = 0;
        // while (j < table.column_count)
        while (j < table.column_count)
        {
            tmp = ft_strlen(table.data[i][j]);
            if (tmp > DEF_COL_WIDTH || tmp > table.columns[j].width)
                table.columns[j].width = tmp;
            j++;
        }
        i++;
    }
    display_title(table);
    display_header(table);
    rows = display_data(table);
    display_footer(rows);
    free(table.title);
}