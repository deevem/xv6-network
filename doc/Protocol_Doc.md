# 网络协议栈部分文档

## tun/tap

我们暂时使用这个玩意来收发包，默认的设备网段是 `10.1.1.2/24`

## mbuf

mbuf 的定义为

```c
struct mbuf {
    struct mbuf* next;
    char*        head;
    uint8_t      len;
    char         buf[MBUF_SIZE];
};
```

大概来说 mbuf 长下面这个样子

```
        <-----len------>
[header][----buffer----][tail]
^       ^
buf     head
```

常见的操作有 `push` `pull` `put` `trim` `alloc` `free` 

前四个操作的意思大概就是

```
           <- push            <- trim
            -> pull            -> put
[-headroom-][------buffer------][-tailroom-]
|----------------MBUF_SIZE-----------------|
```

后两个操作定义如下
```
struct mbuf* mbufalloc(unsigned int hdr_size);
void mbuffree(struct mbuf* m);
```

注意：`mbufalloc` 的参数为 headroom 的大小。

`mbuf_queue` 暂时还没有用到，如果尝试实现 tcp 的话会用到它。

## Ethernet

位于 `ethernet.h` 和 `ethernet.c` 中，收发的函数定义如下

```
void eth_tx(struct mbuf *m, uint16_t ethType);
void eth_rx(struct mbuf *m);
```

目前所有的 ethernet 包发出去的 mac 都是广播地址。

## ARP

位于 `arp.h` 和 `arp.c` 中。

### 接收部分测试方法

```
make arptest
sudo ./arptest
```

打开另一个终端

```
arping -I tap0 10.1.1.5
```

在这个终端中就能看到结果。



## IP

位于 `ip.h` 和 `ip.c` 中，收发的方法为

```
void ip_rx(struct mbuf *m);
void ip_tx(struct mbuf *m, uint8_t protocol, uint32_t dst_ip);
void ip_tx_ready(struct mbuf* m); // 这个方法不改变头中的任何信息，直接计算校验和并发出去
```

## UDP

位置和收发方法同上

### 发送部分测试方法

执行
```
make testudp
sudo ./testudp
```

打开另一个终端

```
python3 py/udp_recv.py
```

在 `testudp` 中硬编码了 `dst_ip`，测试时需要用 `ifconfig` 查看自己网卡的 inet ip 并改一下。

### 接收部分测试方法

```
make recvudp
sudo ./recvudp
```
打开另一个终端
```
python3 py/udp_send.py
```

## ICMP

### 测试方法

```
make icmptest
sudo ./icmptest
```
打开另一个终端
```
ping 10.1.1.5
```
## DNS

### 测试方法

在另一个终端中
```
python3 py/dns_test.py
```

```
make testdns
sudo ./testdns
```

注意这两个有先后顺序，dns 的网址硬编码在 `testdns.c` 中