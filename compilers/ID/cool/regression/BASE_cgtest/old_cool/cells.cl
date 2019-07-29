(* models one-dimensional cellular automaton on a circle of finite radius
   arrays are faked as Strings,
   X's respresent live cells, dots represent dead cells,
   no error checking is done *)
class CellularAutomaton inherits IO is
    population_map : String;
   
    init(map : String) : SELF_TYPE is
        begin
            population_map <- map;
            self;
        end
    end;
   
    print() : SELF_TYPE is
        begin
            out_string(population_map.concat("\n"));
            self;
        end
    end;
   
    num_cells() : Int is
        population_map.length()
    end;
   
    cell(position : Int) : String is
        population_map.substr(position, 1)
    end;
   
    cell_left_neighbor(position : Int) : String is
        if position = 0 then
            cell(num_cells() - 1)
        else
            cell(position - 1)
        fi
    end;
   
    cell_right_neighbor(position : Int) : String is
        if position = num_cells() - 1 then
            cell(0)
        else
            cell(position + 1)
        fi
    end;
   
    (* a cell will live if exactly 1 of itself and it's immediate
       neighbors are alive *)
    cell_at_next_evolution(position : Int) : String is
        if (if cell(position) = "X" then 1 else 0 fi
            + if cell_left_neighbor(position) = "X" then 1 else 0 fi
            + if cell_right_neighbor(position) = "X" then 1 else 0 fi
            = 1)
        then
            "X"
        else
            "."
        fi
    end;
   
    evolve() : SELF_TYPE is
        let position : Int in
        let num : Int <- num_cells() in
        let temp : String in
            begin
                while position < num loop
                    begin
                        temp <- temp.concat(cell_at_next_evolution(position));
                        position <- position + 1;
                    end
                pool;
                population_map <- temp;
                self;
            end
        end end end
    end;
end;

class Main is
    cells : CellularAutomaton;
   
    main() : SELF_TYPE is
        begin
            cells <- (new CellularAutomaton).init("         X         ");
            cells.print();
            let countdown : Int <- 20 in
                while 0 < countdown loop
                    begin
                        cells.evolve();
                        cells.print();
                        countdown <- countdown - 1;
                    end
                pool
            end;
            self;
        end
    end;
end;
