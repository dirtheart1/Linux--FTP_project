完成一个完整的项目需要一系列的步骤和代码编写，我可以给你一个大致的框架和思路，但是具体的实现可能会涉及很多细节和调试。以下是一个简化的示例框架，你可以在此基础上进行代码的编写和完善。

### 服务器端代码示例：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

enum CMD_NO
{
    FTP_CMD_LS = 1024,
    FTP_CMD_GET,
    FTP_CMD_PUT,
    FTP_CMD_BYE,
    FTP_CMD_UNKNOWN = 0xFFFFFFFF
};

struct Command
{
    enum CMD_NO cmd_no;
    int arg_len;
    char arg_data[256];  // Adjust size as needed
};

void *handle_client(void *client_socket_ptr)
{
    int client_socket = *((int *)client_socket_ptr);
    // Read and parse client command
    struct Command cmd;
    // ... Read command and arguments from client_socket into cmd ...

    // Handle command
    switch (cmd.cmd_no)
    {
        case FTP_CMD_LS:
            // Handle ls command
            // Construct and send response packet
            break;

        case FTP_CMD_GET:
            // Handle get command
            // Send file data in response
            break;

        // ... Handle other commands ...

        default:
            // Unknown command
            // Send error response
            break;
    }

    close(client_socket);
    free(client_socket_ptr);
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <PORT>\n", argv[0]);
        return -1;
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    // ... Initialize server_socket ...

    struct sockaddr_in server_addr;
    // ... Setup server_addr ...

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);

    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int *client_socket = malloc(sizeof(int));

        *client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, (void *)client_socket);
        pthread_detach(client_thread);
    }

    close(server_socket);
    return 0;
}
```

### 客户端代码示例：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

enum CMD_NO
{
    FTP_CMD_LS = 1024,
    FTP_CMD_GET,
    FTP_CMD_PUT,
    FTP_CMD_BYE,
    FTP_CMD_UNKNOWN = 0xFFFFFFFF
};

struct Command
{
    enum CMD_NO cmd_no;
    int arg_len;
    char arg_data[256];  // Adjust size as needed
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <IP> <PORT>\n", argv[0]);
        return -1;
    }

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    // ... Initialize client_socket ...

    struct sockaddr_in server_addr;
    // ... Setup server_addr ...

    connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while (1)
    {
        struct Command cmd;
        // ... Construct cmd based on user input ...

        // Send command to server
        // Construct and send packet using cmd structure
        // ...

        // Receive and process response from server
        // ...

        // Break loop and close socket on bye command
        if (cmd.cmd_no == FTP_CMD_BYE)
        {
            break;
        }
    }

    close(client_socket);
    return 0;
}

//============================================================================
//接收数据包之后 解包分析命令

在服务器端接收到数据包后，
你需要对数据包进行解析以提取命令号和参数，
然后根据命令号执行相应的操作。以下是一个示例的代码片段，
展示了如何解析数据包并根据命令号执行不同的操作：


unsigned char recv_buf[1024];
int ret = recv(client_socket, recv_buf, sizeof(recv_buf), 0);
if (ret == -1)
{
    perror("receiving failed!!");
    close(client_socket);
    return -1;
}

// 解析数据包
int i = 0;
unsigned char cmd_start = recv_buf[i++];
if (cmd_start != 0xc0)
{
    // 错误处理：包头不正确
    close(client_socket);
    return -1;
}

int pkg_len = (recv_buf[i++]) |
              (recv_buf[i++] << 8) |
              (recv_buf[i++] << 16) |
              (recv_buf[i++] << 24);

enum CMD_NO cmd_no = (recv_buf[i++]) |
                     (recv_buf[i++] << 8) |
                     (recv_buf[i++] << 16) |
                     (recv_buf[i++] << 24);

// 根据命令号执行操作
switch (cmd_no)
{
    case FTP_CMD_LS:
        // 执行 ls 命令的操作
        // 发送目录文件信息数据包回客户端
        break;

    case FTP_CMD_GET:
        // 执行 get 命令的操作
        // 发送文件内容数据包回客户端
        break;

    // 其他命令的处理类似

    default:
        // 未知命令处理
        break;
}

unsigned char cmd_end = recv_buf[pkg_len - 1];
if (cmd_end != 0xa0)
{
    // 错误处理：包尾不正确
    close(client_socket);
    return -1;
}
```

。在这个代码片段中，我们首先解析了数据包中的包头和包尾，
然后提取了命令号。接下来，我们根据命令号使用 `switch` 
语句执行相应的操作。最后，我们验证了数据包的包尾是否正确。

//===============================================================================


//====================================================================================
当涉及到解析多个命令的数据包时，可以设计一个通用的解析函数。以下是一个示例的通用解析函数，可以适用于不同命令的数据包解析：

```c
enum CMD_NO
{
    FTP_CMD_LS = 1024,
    FTP_CMD_GET,
    FTP_CMD_PUT,
    FTP_CMD_BYE,
    FTP_CMD_UNKNOWN = 0xFFFFFFFF
};

// 数据包信息结构体
struct DataPacket {
    enum CMD_NO cmd_no;
    int resp_len;
    unsigned char result;
    unsigned char *resp_content;
};

// 通用解析函数
struct DataPacket parsePacket(unsigned char *packet) {
    struct DataPacket parsedPacket;
    
    int i = 1; // 跳过包头
    
    // 解析 resp_len
    parsedPacket.resp_len = packet[i++];
    parsedPacket.resp_len |= packet[i++] << 8;
    parsedPacket.resp_len |= packet[i++] << 16;
    parsedPacket.resp_len |= packet[i++] << 24;
    
    // 解析 cmd_no
    parsedPacket.cmd_no = packet[i++];
    parsedPacket.cmd_no |= packet[i++] << 8;
    parsedPacket.cmd_no |= packet[i++] << 16;
    parsedPacket.cmd_no |= packet[i++] << 24;
    
    // 解析 result
    parsedPacket.result = packet[i++];
    
    // 解析 resp_content
    parsedPacket.resp_content = (unsigned char *)malloc(parsedPacket.resp_len - 1);
    memcpy(parsedPacket.resp_content, packet + i, parsedPacket.resp_len - 1);
    
    return parsedPacket;
}

int main() {
    // 示例 response 数据，可根据不同命令的数据包进行替换
    unsigned char response_ls[20] = {
        0xc0, 0x0C, 0x00, 0x00, 0x00, 0x01, 0x04, 0x00,
        0x01, 0x66, 0x69, 0x6C, 0x65, 0x31, 0x2E, 0x74, 0x78, 0x74, 0x20, 0x66
    };
    
    unsigned char response_get[30] = {
        0xc0, 0x15, 0x00, 0x00, 0x00, 0x02, 0x04, 0x00,
        0x01, 0x67, 0x65, 0x74, 0x20, 0x66, 0x69, 0x6C, 0x65, 0x6E, 0x61, 0x6D, 0x65, 0x2E, 0x74, 0x78, 0x74,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xa0
    };

    struct DataPacket parsedPacket_ls = parsePacket(response_ls);
    struct DataPacket parsedPacket_get = parsePacket(response_get);

    printf("Response for LS:\n");
    printf("Command: %u\n", parsedPacket_ls.cmd_no);
    printf("Response Length: %d\n", parsedPacket_ls.resp_len);
    printf("Result: %u\n", parsedPacket_ls.result);
    printf("Response Content: %s\n", parsedPacket_ls.resp_content);

    printf("\nResponse for GET:\n");
    printf("Command: %u\n", parsedPacket_get.cmd_no);
    printf("Response Length: %d\n", parsedPacket_get.resp_len);
    printf("Result: %u\n", parsedPacket_get.result);
    printf("Response Content: %s\n", parsedPacket_get.resp_content);

    free(parsedPacket_ls.resp_content); // Remember to free allocated memory
    free(parsedPacket_get.resp_content);

    return 0;
}
```

在这个示例中，`parsePacket` 函数用于解析数据包，
你可以根据不同命令的数据包结构调用这个函数，
并根据 `parsedPacket` 结构体中的信息进行后续处理。不
同的命令只需要提供对应的 `response` 数据即可。注意，
在使用完成后要释放分配的内存。

//============================================================================

备份

DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    char result[MAX_LENGTH] = "";

    if ((dp = opendir(dir)) == NULL)
    {
        fprintf(stderr, "显示文件列表失败，无法打开路径：%s\n", dir);
        send_simple(sockfd, FAILURE);
        return -1;
    }
    chdir(dir);
    while ((entry = readdir(dp)) != NULL)
    {
        // 忽略.和..
        if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
            continue;
        strcat(result, entry->d_name);
        // printf("result += %s\n", entry->d_name);
        strcat(result, "\t");
    }
    struct ftpmsg msg;
    msg.type = SUCCESS;
    msg.len = strlen(result) + 1;
    msg.data = result;
    send_msg(sockfd, &msg);
    printf("显示文件列表成功\n");
    // printf("%s\n", result);
    closedir(dp);
    return 0;

		
		

    //============================================================================