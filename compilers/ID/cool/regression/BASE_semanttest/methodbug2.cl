class Main is
    f() : Object is 1 end;
    main() : Object is
        let f : Int in self@Main.f() end
    end;
end;
