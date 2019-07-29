class Main inherits IO is
        x: Int <- 0;
        y: Object;
        main(): Object is
                while x = 1 loop
                case x of
                        y: Int =>
                        begin
                        let z: Int <- 2 in
                                if z = 3 then true else false fi
                        end;
                        x <- y;
                        end;

                        y: Object => abort();
                esac
                pool
        end;
end;
