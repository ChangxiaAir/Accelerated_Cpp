#include<memeory>

template <class T> class Vec {
public:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef size_t size_type;
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;

    Vec() { create(); }
    explicit Vec(size_type n, const T& t=T()) { create(n, t); }
    Vec& operator=(const Vec&);
    ~Vec() { uncreate(); }

    T& operator[] (size_type i) { return data[i]; }
    const T& operator[] (size_type i) { return data[i]; }

    void push_back(const T& t){
        if(avail == limit){
            grow();
        }
        uncheck_append(t);
    }

    size_type size() const { return avail - data;}
    iterator begin() {return data;}
    const_iterator begin() {return data;}

    iterator end() {return avail;}
    const_iterator end() {return avail;}

private:
    iterator data;  //Vec中的首元素
    iterator avail; //Vec中末元素后面的一个元素
    iterator limit; //新分配内存中末元素后面的一个元素

    //内存分配工具
    allocator<T> alloc; //控制内存分配的对象

    //为底层的数组分配空间并对他进行初始化
    void create();
    void create(size_type, const T&);
    void create(const_iterator, const_iterator);

    //删除数组中的元素并释放其占用的内存
    void uncreate();

    //支持push_back的函数
    void grow();
    void unchecked_append();
};

template <class T> void Vec<T>::create
{
    data = avail = limit = 0;
}

template <class T> void Vec<T>::create(size_type n, const T& val)
{
    data = alloc.allocate(n);
    limit = avail = data+n;
    uninitialized_fill(data, limit, val);//该函数将第三个参数指向的
                                        //对象内容复制到前两个参数指向的尚未被初始化的内存空间

}

template <class T> void Vec<T>::create(const_iterator i, const_iterator j)
{
    data = alloc.allocate(j-i);
    limit = avail = uninitialized_copy(i, j, data);
}

template <class T> void Vec<T>::uncreate()
{
    if(data){
        //（以相反的顺序）删除构造函数生成的元素
        iterator it = avail;
        while(it != data)
            alloc.destory(--it);
    //返回占用的所有内存空间
    alloc.deallocate(data, limit-data);
    }
    //重置指针以表明Vec类型对象为空
    data=avail=limit=0;
}

template <class T> void Vec<T>::grow()
{
    //在扩展对象大小时，为对象分配实际使用的两倍大小的内存空间
    size_type new_size = max(2*(limit-data), ptrdiff_t(1));

    //分配新的内存空间并将已存在的对象元素内容复制到新内存中
    iterator new_data=alloc.allocate(new_size);
    iterator new_avail=uninitialzed_copy(data,avail,new_data);

    //返回原来的内存空间
    uncreate();

    //重置指针，使其指向新分配的内存空间
    data = new_data;
    avail = new_avail;
    limit = data + new_size;
}
//假设avail指向一片新分配但尚未被初始化的内存空间
template<class T> Vec<T>::unchecked_append(const T& val)
{
    alloc.construct(avail++, val);
}