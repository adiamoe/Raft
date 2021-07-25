## Raft by C++

### 实现细节

1. 基于boost的asio库和开源json库实现网络传输
2. 通过timer和异步回调实现状态的动态转换
3. 利用Random库最大化避免follower同时转换为candidate导致一个任期内没有leader

### TODO

1. 新节点的加入机制
2. 日志持久化，现在仅仅是将数据储存在内存中
3. 极端情况下的正确性校验
