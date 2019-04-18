### Simple NFA-based regex engine.

---

#### 0.0.1

-  支持匹配字符范围 ASCII
- 支持重复符 *，？，+（有限次匹配待实现）
- 支持范围表示的字符 eg. [A-Z]可以表示大写字母中任意一个
- 支持并联运算(|)和串联运算
- match_all_text接口，若整个文本匹配正则表达式返回true，否则返回false
- match_points接口，返回文本的index，表示文本的[0,index]能匹配正则表达式

下面是一个简单的实例。

```c++
match_all_text("abbacadaaefa", R"(((ab*|ac*)*|ad*|ef)*)");//返回true.
match_points("abbacadaaefa",R"(((ab*|ac*)*|ad*|ef)*)");
		//返回所有的index，表示任意index前的字串都能匹配
```



#### 0.0.2

**待实现**

- 重复符的有限次匹配(Finish!)
- 0.0.1版本的一些数据结构的优化

#### 0.0.3-???

- 正向预查，反向预查，匿名捕获，命名捕获，命名检查，边界，贪婪，非贪婪（能不能实现看**缘分**吧：）