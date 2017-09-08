#pragma comment(lib, "libmySQL.lib")
#pragma comment(lib, "ws2_32.lib")


#include <iostream>
#include <string>

//MySQL ���� �������
#include <my_global.h>
#include <mysql.h>

using namespace std;

int main(int argc, char **argv)
{

	string str_query;
	string str_input_parking_id;
	string str_input_parking_level;
	int ninput_parking_id;
	int ninput_parking_level;

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int query_stat;


	char *server = "localhost";
	char *user = "root";
	char *password = "rkdxorn"; //root���� ��й�ȣ
	char *database = "test"; //������ �����ͺ��̽�
	int port = 3307; //��Ʈ��ȣ

	conn = mysql_init(NULL);

	/* DB ���� */
	if (!mysql_real_connect(conn, server,
		user, password, database, port, NULL, 0)) {
		fprintf(stderr, "%s ", mysql_error(conn));
		exit(1);
	}

	//�� ������
	query_stat = mysql_query(conn, "select * from parking");
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
		return 1;
	}
	
	/* send SQL query */
	//parking ���̺� ��� �ڷ� �˻�
	if (mysql_query(conn, str_query.c_str())) {
		fprintf(stderr, "%s ", mysql_error(conn));
		exit(1);
	}
	res = mysql_store_result(conn);

	/* close connection */
	mysql_free_result(res);
	mysql_close(conn);
	return 0;
}
