#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 0
#define BAD_ARGUMENT -1
#define BAD_PATH -2

struct path_obj {
  char *path;
};

int argument_handler(int argc, char *argv[], struct path_obj *path_o);
int get_arrays(struct path_obj *path_o, int* l_ptr[], int* r_ptr[]);
int count_file_rows(struct path_obj *path_o);
void sort(int arr_s, int *arr[]);
int get_distance(int *l_arr[], int *r_arr[], int arr_s);
int calc_simularity(int *l_arr[], int *r_arr[], int arr_s);

int main(int argc, char *argv[])
{
  struct path_obj *path_o = malloc(sizeof(struct path_obj));
  int *l_arr, *r_arr, arr_s;

  if (argument_handler(argc, argv, path_o) == BAD_ARGUMENT) {
    printf("Bad argument.");
    return BAD_ARGUMENT;
  }

  arr_s = get_arrays(path_o, &l_arr, &r_arr);

  if (arr_s == BAD_PATH) {
    free(path_o->path);
    free(path_o);

    free(l_arr);
    free(r_arr);

    return BAD_PATH;
  }

  sort(arr_s, &l_arr);
  sort(arr_s, &r_arr);

  int distance = get_distance(&l_arr, &r_arr, arr_s);
  int simularity = calc_simularity(&l_arr, &r_arr, arr_s);

  printf("Distance: %d\n", distance);
  printf("Simularity: %d\n", simularity);

  free(l_arr);
  free(r_arr);

  free(path_o->path);
  free(path_o);

  return SUCCESS;
}

int calc_simularity(int *l_arr[], int *r_arr[], int arr_s) {
  int total_simularity, simularity, i, j, counter;

  total_simularity = 0;
  counter = 0;

  for(i = 0; i <  arr_s; ++i) {
    for(j = 0; j < arr_s; ++j) {
      if((*l_arr)[i] == (*r_arr)[j]) {
        counter++;
      }
    }

    simularity = (*l_arr)[i] * counter;
    total_simularity += simularity;
    counter = 0;
  }

  return total_simularity;
}

int get_distance(int *l_arr[], int *r_arr[], int arr_s) {
  int total_distancce, distance, i;

  total_distancce = 0;

  for(i = 0; i < arr_s; ++i) {
    distance = (*l_arr)[i] - (*r_arr)[i];
    if(distance < 0) {
      distance *= -1;
    }

    total_distancce += distance;
  }

  return total_distancce;
}

void sort(int arr_s, int *arr[]) {
  int i;

  for (i = 1; i < arr_s; ++i) {
    int x = (*arr)[i];
    int j = i - 1;

    while (j >= 0 && (*arr)[j] > x) {
      (*arr)[j + 1] = (*arr)[j];
      j = j - 1;
    }

    (*arr)[j + 1] = x;
  }

  for(i = 0; i + 1 < arr_s; ++i) {
    if((*arr)[i + 1] < (*arr)[i]) {
      printf("%d\n", i);
    }
  }
}

int argument_handler(int argc, char *argv[], struct path_obj *path_o) {
  if (argc < 2) {
    return BAD_ARGUMENT;
  }

  path_o->path = malloc(sizeof(char) * (strlen(argv[1]) + 1));
  strcpy(path_o->path, argv[1]);
  
  return SUCCESS;
}

int get_arrays(struct path_obj *path_o, int* l_arr[], int* r_arr[]) {

  FILE *file;
  char *buffer, *token;
  size_t size;
  int temp, arr_c, arr_s;

  arr_c = count_file_rows(path_o);

  if (arr_c == BAD_PATH) {
    return BAD_PATH;
  }
  
  *l_arr = (int*) malloc(sizeof(int) * arr_c);
  *r_arr = (int*) malloc(sizeof(int) * arr_c);

  file = fopen(path_o->path, "r");

  if (file == NULL) {
    free(buffer);
    free(file);
    return BAD_PATH;
  }

  arr_s = 0;
  size = 0;
  buffer = NULL;

  while (getline(&buffer, &size, file) != -1) {
    token = strtok(buffer, " ");
    temp = atoi(token);
    (*l_arr)[arr_s] = temp;

    token = strtok(NULL, " ");
    temp = atoi(token);
    (*r_arr)[arr_s] = temp;

    arr_s = arr_s + 1;
  }

  free(buffer);
  fclose(file);

  return arr_s;
}

int count_file_rows(struct path_obj *path_o) {
  FILE *file;
  int count;
  char *buffer;
  size_t size;

  file = fopen(path_o->path, "r");

  if (file == NULL) {
    return BAD_PATH;
  }

  count = 0;
  size = 0;
  buffer = NULL;

  while (getline(&buffer, &size, file) != -1) {
    count = count + 1;
  } 

  fclose(file);
  free(buffer);
  return count;
}
