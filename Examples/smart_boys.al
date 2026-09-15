static object Student:
    name : string

@initialize
function smart_boys(argc : int.32, argv : char.8**) -> int.32:
    def student_quantity : int'32
    def students : Student*
    inputln! << &student_quantity << ">> {int'32}"
    students = |student_quantity * size(Student)|
    return 0
