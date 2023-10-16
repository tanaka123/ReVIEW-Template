
= 章タイトル

ここからすべてが始まる！


ここからすべてが始まる！

ここから,すべてが始まる。
aa
a,ここからすべてが始まる。,

a,ここからすべてが始まる。,



//list[hello.rb][article.re]{
#@mapfile(scripts/hello.rb)
#!/usr/bin/env ruby


class Hello
    def hello(name)
      print "hello, #{name}!\n"
    end
  end

  if __FILE__ == $0
    Hello.new.hello("world")
  end
#@end
//}

