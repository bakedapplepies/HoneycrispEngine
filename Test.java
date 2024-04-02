import java.util.ArrayList;

class A {}
class B extends A {}

public class Test
{
    public static void main(String[] args)
    {
        String str = "compiler";
        System.out.println(str.substring(4, str.length()) + str.substring(0, 3));
    }
}