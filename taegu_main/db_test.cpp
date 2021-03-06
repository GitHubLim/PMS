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
    ) TYPE = MYISAM ;" //DB에 테이블 만듬
#define SQL_INSERT_RECORD "INSERT INTO `parking` ( `ID` , `level` ) \
    VALUES (\
    NULL , '%d'\
    );" //DB에 값 넣기
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

	// DB 연결 (포트 다르게해야함)
	connection = mysql_real_connect(&conn, DB_HOST,
		DB_USER, DB_PASS, DB_NAME, 3307, (char *)NULL, 0);
	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}
	/* 초기화용....
	// 테이블 삭제
	query_stat = mysql_query(connection, SQL_DROP_TABLE);
	if (query_stat != 0)
	{
	fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
	return 1;
	}
	*/
	// 테이블 생성
	query_stat = mysql_query(connection, SQL_CREATE_TABLE);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}

	// 레코드 삽입
	for (int i = 0; i<20; i++)
	{
		sprintf(query, SQL_INSERT_RECORD,&level[i]);//삽입 쿼리 부분, 보충필요
		query_stat = mysql_query(connection, query);
		if (query_stat != 0)
		{
			fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
			return 1;
		}
	}

	// 셀렉트
	query_stat = mysql_query(connection, SQL_SELECT_RECORD);
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
		return 1;
	}

	// 결과 출력
	sql_result = mysql_store_result(connection);
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		printf("%2s %2s \n", sql_row[0], sql_row[1]);//레코드 값 출력
	}
	mysql_free_result(sql_result);//포인터 해제

	// DB 연결 닫기
	mysql_close(connection);
	return 0;
}