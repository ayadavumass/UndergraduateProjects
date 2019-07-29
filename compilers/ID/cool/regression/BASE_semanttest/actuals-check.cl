class Main is
  main():Object is 2 end;
  c: Main <- let x:Int <- "hi" in 2 end;
  d: Main <- c@M.foo(let x:Int <- "hi" in 2 end);
end;
