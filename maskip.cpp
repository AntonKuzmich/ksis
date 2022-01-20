#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>

#include <locale>
#include <iostream>
#include <sstream>

using namespace std;



bool CheckAddress(char* ip_)
{
	int points = 0;
	int numbers = 0;
	char* buff;
	char dig;
	buff = new char[4];

	for (int i = 0; ip_[i] != '\0'; i++)
	{
		dig = ip_[i];

		if (dig <= '9' && dig >= '0')
		{

			if (numbers > 3) { return false; }

			buff[numbers++] = dig;


		}
		else
		{
			if (dig == '.')
			{
				buff[numbers] = 0;
				if (atoi(buff) > 255) { return false; }
				if (numbers == 0) { return false; }
				numbers = 0;
				points++;

			}
			else return false;

		}
	}

	if (points != 3) { return false; }
	if (numbers == 0 || numbers > 3) { return false; }

	return true;
}


unsigned long CharToULong(char* ip)
{
	unsigned long  out = 0;
	char buff[4];
	for (int i = 0, j = 0, k = 0; ip[i] != '\0'; i++)
	{
		if (ip[i] != '.')	buff[j++] = ip[i];

		if (ip[i] == '.' || ip[i + 1] == '\0')
		{

			out = out << 8;

			buff[j] = 0;
			if (atoi(buff) > 255) return 0;


			out += atoi(buff);
			k++;
			j = 0;
		}

	}

	return out;
}



string ip2a(unsigned long IP)
{
	stringstream  ss;

	unsigned char* ip = (unsigned char*)&IP;

	ss << (int)ip[3] << '.' << (int)ip[2] << '.' << (int)ip[1] << '.' << (int)ip[0];

	return ss.str();

}

bool checkMaskIP(unsigned long mask)
{
	unsigned long test = 0x80000000;
	bool endmask = false;

	for (int i = 0; i < 32; i++)
	{
		if (endmask == true && (mask & test) == test) return false;

		if ((mask & test) != test) { endmask = true; }

		test = test >> 1;
	}

	return true;
}

int main()
{

	unsigned long ip;
	unsigned long mask;


	unsigned long host;
	unsigned long subnet;
	unsigned long broadcast;

	char s_ip[16];
	char s_mask[16];

	setlocale(LC_ALL, "RUS");

	do
	{
		cout << "IP:  ";
		cin >> s_ip;

		if (CheckAddress(s_ip) == true) break;

		cout << "Введен неверный адрес!" << endl;

	} while (true);

	ip = CharToULong(s_ip);



	do {

		cout << "MASK:  ";
		cin >> s_mask;

		if (CheckAddress(s_mask) == true)
		{
			mask = CharToULong(s_mask);

			if (checkMaskIP(mask) == true) break;

			cout << "В маске нет непрерывности единиц !" << endl;
		}
		cout << "Введена неверная маска !" << endl;

	} while (true);




	subnet = ip & mask;
	host = ip & ~mask;
	broadcast = ip & mask | ~mask;

	cout << "Address IP: " << ip2a(ip) << endl;
	cout << "Address mask: " << ip2a(mask) << endl;
	cout << "Host ID:" << ip2a(host) << endl;
	cout << "Subnet ID:" << ip2a(subnet) << endl;
	cout << "Broadcast:" << ip2a(broadcast) << endl;

	system("pause");
	return 0;

}
