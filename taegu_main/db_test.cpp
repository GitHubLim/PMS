#include <mysql.h>
#include <string.h>
#include <stdio.h>

#pragma comment(lib, "libmySQL.lib") 

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "rkdxorn"
#define DB_NAME "test"

#define SQL_CREATE_TABLE "CREATE TABLE `parking` (\
    `id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY ,\
    `level` INT NULL \
    ) TYPE = MYISAM ;" //DB�� ���̺� ����
#define SQL_INSERT_RECORD "INSERT INTO `parking` ( `ID` , `level` ) \
    VALUES (\
    NULL , '%d'\
    );" //DB�� �� �ֱ�
#define SQL_SELECT_RECORD "SELECT * FROM `parking`"
#define SQL_DROP_TABLE "DROP TABLE `parking`"

int main()
{
	MYSQL *connection = NULL, conn;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;

	char query[255];

	mysql_init(&conn);

	// DB ���� (��Ʈ �ٸ����ؾ���)
	connection = mysql_real_connect(&conn, DB_HOST,
		DB_USER, DB_PASS, DB_NAME, 3307, (char *)NULL, 0);
	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}
	/* �ʱ�ȭ��....
	// ���̺� ����
	query_stat = mysql_query(connection, SQL_DROP_TABLE);
	if (query_stat != 0)
	{
	fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
	return 1;
	}
	*/
	// ���̺� ����
	query_stat = mysql_query(connection, SQL_CREATE_TABLE);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}

	// ���ڵ� ����
	for (int i = 0; i<20; i++)
	{
		sprintf(query, SQL_INSERT_RECORD,&level[i]);//���� ���� �κ�, �����ʿ�
		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			return 1;
		}
	}

	// ����Ʈ
	query_stat = mysql_query(connection, SQL_SELECT_RECORD);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}

	// ��� ���
	sql_result = mysql_store_result(connection);
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		printf("%2s %2s \n", sql_row[0], sql_row[1]);//���ڵ� �� ���
	}
	mysql_free_result(sql_result);//������ ����

	// DB ���� �ݱ�
	mysql_close(connection);
	return 0;
}