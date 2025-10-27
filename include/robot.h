struct Robot;

void forward(Robot*);
void left(Robot*);
void right(Robot*);
bool is_at_marker(Robot*);
bool can_move_forward(Robot*);
void pickup_marker(Robot*);
void drop_marker(Robot*);
int get_marker_count(Robot*);