#ifndef rbtree_FCF1F854_7F36_47ED_9A40_902D4D7BAB54
#define rbtree_FCF1F854_7F36_47ED_9A40_902D4D7BAB54

struct rb_node {
	struct rb_node *parent;
	struct rb_node *rb_right;
	struct rb_node *rb_left;
	bool color;
};

struct rb_root {
	struct rb_node *rb_node;
};

#define rb_parent(r)   ((struct rb_node *) (r)->parent)



#endif // rbtree_FCF1F854_7F36_47ED_9A40_902D4D7BAB54
