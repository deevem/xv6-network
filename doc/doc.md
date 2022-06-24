## 用于展示的文档

### 完成的功能

- e1000 driver
- TCP/IP 协议栈
    - Ethernet、IP、ARP、ICMP、UDP、TCP、DNS
- socket 及相关的系统调用
    - connect connect_icmp read write
    - connect_tcp bind_tcp listen_tcp accept_tcp
- 应用层程序
    - ping nslookup
    - Group
    - TODO：wget

### 网络协议栈部分

我们实现一个比较完整的 TCP/IP 协议栈，包括

- 链路层：Ethernet 协议
- 网络层：IP 协议、ARP 协议、ICMP 协议
- 传输层：UDP 协议，TCP 协议
- 应用层：DNS 协议

### 

Qemu 提供了虚拟的 PCI 总线以及连接在 PCI 总线上的 E1000 网卡。可以通过 DMA 的方式与网卡交互，完成包的收发功能，同时 PCI 提供了。

