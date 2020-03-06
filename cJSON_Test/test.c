/*
  Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

/* Used by some code below as an example datatype. */
struct record
{
	const char *precision;
	double lat;
	double lon;
	const char *address;
	const char *city;
	const char *state;
	const char *zip;
	const char *country;
};


/* Create a bunch of objects as demonstration. */
static int print_preallocated(cJSON *root)
{
    /* declarations */
    char *out = NULL;
    char *buf = NULL;
    char *buf_fail = NULL;
    size_t len = 0;
    size_t len_fail = 0;

    /* formatted print */
    out = cJSON_Print(root);

    /* create buffer to succeed */
    /* the extra 5 bytes are because of inaccuracies when reserving memory */
    len = strlen(out) + 5;
    buf = (char*)malloc(len);
    if (buf == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    /* create buffer to fail */
    len_fail = strlen(out);
    buf_fail = (char*)malloc(len_fail);
    if (buf_fail == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    /* Print to buffer */
    if (!cJSON_PrintPreallocated(root, buf, (int)len, 1)) {
        printf("cJSON_PrintPreallocated failed!\n");
        if (strcmp(out, buf) != 0) {
            printf("cJSON_PrintPreallocated not the same as cJSON_Print!\n");
            printf("cJSON_Print result:\n%s\n", out);
            printf("cJSON_PrintPreallocated result:\n%s\n", buf);
        }
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    /* success */
    printf("%s\n", buf);

    /* force it to fail */
    if (cJSON_PrintPreallocated(root, buf_fail, (int)len_fail, 1)) {
        printf("cJSON_PrintPreallocated failed to show error with insufficient memory!\n");
        printf("cJSON_Print result:\n%s\n", out);
        printf("cJSON_PrintPreallocated result:\n%s\n", buf_fail);
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    free(out);
    free(buf_fail);
    free(buf);
    return 0;
}

/* Create a bunch of objects as demonstration. */
static void create_objects(void)
{
	cJSON *root = NULL;
	cJSON *fmt = NULL;
	cJSON *img = NULL;
	cJSON *thm = NULL;
	cJSON *fld = NULL;
	int i = 0;

	const char *strings[7] =
	{
		"Sunday",
		"Monday",
		"Tuesday",
		"Wednesday",
		"Thursday",
		"Friday",
		"Saturday"
	};
	/* Our matrix: */
	int numbers[3][3] =
	{
		{0, -1, 0},
		{1, 0, 0},
		{0, 0, 1}
	};
	/* Our "gallery" item: */
	int ids[4] = {116, 943, 234, 38793};
	/* Our array of "records": */
	struct record fields[2] =
	{
		{
			"zip",
			37.7668,
			-1.223959e+2,
			"",
			"SAN FRANCISCO",
			"CA",
			"94107",
			"US"
		},
		{
			"zip",
			37.371991,
			-1.22026e+2,
			"",
			"SUNNYVALE",
			"CA",
			"94085",
			"US"
		}
	};
	volatile double zero = 0.0;

	/* Here we construct some JSON standards, from the JSON site. */

	/* Our "Video" datatype: */
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));
	cJSON_AddItemToObject(root, "format", fmt = cJSON_CreateObject());
	cJSON_AddStringToObject(fmt, "type", "rect");
	cJSON_AddNumberToObject(fmt, "width", 1920);
	cJSON_AddNumberToObject(fmt, "height", 1080);
	cJSON_AddFalseToObject(fmt, "interlace");
	cJSON_AddNumberToObject(fmt, "frame rate", 24);

	/* Print to text */
	if (print_preallocated(root) != 0)
	{
		cJSON_Delete(root);
		exit(EXIT_FAILURE);
	}
	cJSON_Delete(root);

	/* Our "days of the week" array: */
	root = cJSON_CreateStringArray(strings, 7);
	if (print_preallocated(root) != 0)
	{
		cJSON_Delete(root);
		exit(EXIT_FAILURE);
	}
	cJSON_Delete(root);

	/* Our matrix: */
	root = cJSON_CreateArray();
	for (i = 0; i < 3; i++)
	{
		cJSON_AddItemToArray(root, cJSON_CreateIntArray(numbers[i], 3));
	}
	/* cJSON_ReplaceItemInArray(root, 1, cJSON_CreateString("Replacement")); */

	if (print_preallocated(root) != 0) {
		cJSON_Delete(root);
		exit(EXIT_FAILURE);
	}
	cJSON_Delete(root);

	/* OUT "gallary" item: */
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "Image", img = cJSON_CreateObject());
	cJSON_AddNumberToObject(img, "Width", 800);
	cJSON_AddNumberToObject(img, "Height", 600);
	cJSON_AddStringToObject(img, "Tittle", "View from 15th Floor");
	cJSON_AddItemToObject(img, "Thumbnail", thm = cJSON_CreateObject());
	cJSON_AddStringToObject(thm, "Url", "http:/*www.example.com/image/481948843");
	cJSON_AddNumberToObject(thm, "Height", 125);
	cJSON_AddNumberToObject(thm, "Width", 100);
	cJSON_AddItemToObject(img, "IDs", cJSON_CreateIntArray(ids, 4));

	if (print_preallocated(root) != 0)
	{
		cJSON_Delete(root);
		exit(EXIT_FAILURE);
	}
	cJSON_Delete(root);

	/* Our array of "records": */
	root = cJSON_CreateArray();
	for (i = 0; i < 2; i++)
	{
		cJSON_AddItemToArray(root, fld = cJSON_CreateObject());
		cJSON_AddStringToObject(fld, "precision", fields[i].precision);
		cJSON_AddNumberToObject(fld, "Latitude", fields[i].lat);
		cJSON_AddNumberToObject(fld, "Longitude", fields[i].lon);
		cJSON_AddStringToObject(fld, "Address", fields[i].address);
		cJSON_AddStringToObject(fld, "City", fields[i].city);
		cJSON_AddStringToObject(fld, "State", fields[i].state);
		cJSON_AddStringToObject(fld, "Zip", fields[i].zip);
		cJSON_AddStringToObject(fld, "Country", fields[i].country);
	}
	/* cJSON_ReplaceItemInObject(cJSON_GetArrayItem(root, 1), "City", cJSON_CreateIntArray(ids, 4)); */

	if (print_preallocated(root) != 0)
	{
		cJSON_Delete(root);
		exit(EXIT_FAILURE);
	}
	cJSON_Delete(root);

	root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "Number", 1.0/zero);
	if (print_preallocated(root) != 0)
	{
		cJSON_Delete(root);
		exit(EXIT_FAILURE);
	}
	cJSON_Delete(root);
}

//自测试函数声明
void cjson_test2();
void cjson_test2_decode();

int CJSON_CDECL main(void)
{
    /* print the version */
	printf("version: %s\r\n", cJSON_Version());

    /* Now some samplecode for building objects concisely: */
	//create_objects();//官方测试示例

	cjson_test2();//生成json
	cjson_test2_decode();//解析json

	system("pause");
    return 0;
}

void cjson_test2()
{
/*
	创建如下JSON内容
	{
	"name": "Tom",
	"age": 24,
	"love": [ "basketball","swim",{"games": ["LOL","CF"]} ]
	}
*/
	cJSON *arr = NULL;

	cJSON *root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "name", "Tom");
	cJSON_AddNumberToObject(root, "age", 24);

	cJSON_AddItemToObject(root, "love", arr = cJSON_CreateArray());
	cJSON_AddItemToArray(arr, cJSON_CreateString("basketball"));
	cJSON_AddItemToArray(arr, cJSON_CreateString("swim"));

	cJSON *sub = NULL;
	cJSON_AddItemToArray(arr, sub = cJSON_CreateObject());
	cJSON *subarr = NULL;
	cJSON_AddItemToObject(sub, "games", subarr = cJSON_CreateArray());
	cJSON_AddItemToArray(subarr, cJSON_CreateString("LOL"));
	cJSON_AddItemToArray(subarr, cJSON_CreateString("CF"));

	if (print_preallocated(root) != 0)
	{
		cJSON_Delete(root);
		exit(EXIT_FAILURE);
	}
	//cJSON_Delete(root);

	char *buf = cJSON_Print(root);////将root结构体转换成buf
	FILE *fp = fopen("new.json", "w+");
	fwrite(buf, 1, strlen(buf), fp);
	free(buf);
	fclose(fp);
	cJSON_Delete(root);

	/*
		运行结果：
		version: 1.7.12
		{
		"name": "Tom",
		"age":  24,
		"love": ["basketball", "swim", {
		"games":        ["LOL", "CF"]
		}]
	*/
}

void cjson_test2_decode()
{
	/*
	解析此json结构
	{
	"name": "Tom",
	"age": 24,
	"love": [ "basketball","swim",{"games": ["LOL","CF"]} ]
	}

	代码执行结果：
		name = Tom
		age = 24
		love: [basketball, swim, { games:[LOL CF ] }]
	*/

	FILE* fp;
	char *content; //文件内容
	long len;
	fp = fopen("new.json", "rb");
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	printf("file len = %ld\n", len);
	fseek(fp, 0, SEEK_SET);
	content = (char *)malloc(len+1);
	fread(content, 1, len, fp);
	fclose(fp);

	cJSON *json = cJSON_Parse(content); //string -->json
	cJSON *name = cJSON_GetObjectItem(json, "name");
	printf("name = %s\n", name->valuestring);
	cJSON *age = cJSON_GetObjectItem(json, "age");
	printf("age = %d\n", age->valueint);

	cJSON *love = cJSON_GetObjectItem(json, "love");
	if (love == NULL)
	{
		printf("not find love\n");
		exit(1);
	}
	printf("love: [");
	for (int i = 0; i < cJSON_GetArraySize(love); i++)
	{
		cJSON *node = cJSON_GetArrayItem(love, i);
		if (node->type == cJSON_String)
		{
			printf("%s, ", node->valuestring);
		}
		else
		{
			cJSON *Arr = cJSON_GetObjectItem(node, "games");
			if (Arr == NULL)
			{
				printf("not find games\n");
				exit(1);
			}
			printf("{ games:[");
			for (int i = 0; i < cJSON_GetArraySize(Arr); i++)
			{
				cJSON *node = cJSON_GetArrayItem(Arr, i);
				if (node->type == cJSON_String)
				{
					printf("%s ", node->valuestring);
				}
				else
				{
					printf("node->type != cJSON_String\n");
				}
			}
			printf("] }");
		}
	}
	printf("]");
	printf("\n");
}

