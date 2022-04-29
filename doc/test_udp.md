## UDP 发送测试

在 `net` 下 `make testudp`，执行 `sudo ./testudp`

打开一个终端，执行
```
sudo ip l s tap0 up
```

在终端中抓包

```
sudo tcpdump -nnt -i tap0 -vv
```