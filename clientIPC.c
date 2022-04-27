// Client side C/C++ program to demonstrate Socket
// programming
//https://stackoverflow.com/questions/57730441/sockets-programming-sending-and-receiving-different-data-to-different-clients-i
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
/*
char* msg="<DERControlList href=\"/sep2/A1/derp/1/derc\" subscribable=\"1\" all=\"1\" results=\"1\" xmlns=\"urn:ieee:std:2030.5:ns\">
				<DERControl href=\"/sep2/A1/derp/1/derc/1\" replyTo=\"/rsps/1/rsp\" responseRequired=\"03\">
					<mRID>D0000001</mRID>
					<description>Scheduled DERC</description>
					<creationTime>1514838000</creationTime>
					<EventStatus> 
						<currentStatus>0</currentStatus>
						<dateTime>1514838000</dateTime>
						<potentiallySuperseded>false</potentiallySuperseded>
					</EventStatus>
					<interval>
						<duration>3600</duration>
						<start>1514926800</start>
					</interval> 
					<DERControlBase> 
						<opModMaxLimW>9500</opModMaxLimW> 
					</DERControlBase> 
				</DERControl>
			</DERControlList>";*/
int socket_verify(int sock, struct sockaddr_in serv_addr);

int socket_send(int sock, char* hello, char *buffer,int valread);

int main(int argc, char const* argv[])
{
	int sock = 0, valread; //initialize sock and valread
	struct sockaddr_in serv_addr;
	char* msg = "Client<place holder for der control plain text>"; // message to be sent
	char buffer[1024] = { 0 };

	sock = socket(AF_INET, SOCK_STREAM, 0);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	int sv=socket_verify(sock, serv_addr);

	//sends the message to the server
	int sendResult=socket_send(sock, msg, buffer, valread);
	return 0;
}

int socket_send(int sock, char* msg, char *buffer, int valread) {
	send(sock, msg, strlen(msg), 0);
	printf("client message sent\n");
	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);
}

int socket_verify(int sock, struct sockaddr_in serv_addr) {
		//if sock < 0 socket creation error
	if (sock < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<= 0) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
}