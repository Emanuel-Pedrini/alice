static object Cat:
    name : string
    health : float'64
    age : int'32
    color : {int'32, int'32, int'32}
    
    @method
    function new(
        name : string,
        age : int'32,
        color : {int'32, int'32, int'32}
    ) -> self:
        return self(
            name =: name,
            health =: 1.0, 
            age =: age, 
            color =: color)

    function meow(&self) -> void:
        println! << "{} said MEOW!" << self.name
        return

function veterinary(cat : &Cat):
    if cat.health >= 1.0 ? 1.1: 1.0

@initialize 
function object(argc : int'32, argv : char'8**) -> int'32:
    def X : float'32 = 4.0 
    def Larry : Cat = Cat::new(string::new("Larry"), <int'32> X, {255, 0, 0})
    def July : Cat = Cat::new(string::new("July"), <int'32> (X + 1.0), {0, 255, 0})
    July.health -= 0.5

    print! << "{}\n" << size(Larry)
    Larry.meow()
    July.meow()
    veterinary(&July)
    
    return 0