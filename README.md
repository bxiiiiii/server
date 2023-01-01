# Hello, it's Xserver！
> Xserver是一个运行在Linux平台上的Http服务器

## Xerver1.0

- 采用基于事件驱动的 Reactor 模式
- 采用基于 one loop per thread + thread pool 并发模型
- 采用Linux下独有的 epoll IO复用机制
- 实现基于前后端分离、多缓冲区的高效多线程异步日志库
- 采用状态机实现 http 解析，支持静态资源的访问
- 封装用户态 Buffer类，提高稳定性
- 采用 RAII 思想管理资源，避免内存泄露

## 构建及使用
- 确保安装 cmake
``` shell
git clone git@github.com:bxiiiiii/server.git
cd server
mkdir build
cd build
cmake ..
make
./server
```
## 后续
- 通过Webbench的压测得到的数据来看，与ngix、apache还是有一些性能差距。考虑到没有在真实的网络环境中进行传输，压测数据只能做一个简单的参考，这里就不把分析数据列出来了。
- 后续会进行一些性能分析和优化以及一些新功能的开发，包括加入一些C++新版本的特性、IO机制等
