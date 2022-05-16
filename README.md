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