import java.util.EmptyStackException;

public class StackTest {

    private static int testsRun = 0;
    private static int testsPassed = 0;

    private static void pass(String name) {
        testsPassed++;
        System.out.println("  " + name + " - PASS");
    }

    private static void fail(String name, String msg) {
        System.out.println("  " + name + " - FAIL (" + msg + ")");
    }

    private static void testDefaultConstruction() {
        testsRun++;
        Stack s = new Stack();
        if (s.size() != 0) {
            fail("new stack starts empty", "size != 0");
            return;
        }
        if (!s.isEmpty()) {
            fail("new stack starts empty", "not empty");
            return;
        }
        pass("new stack starts empty");
    }

    private static void testExplicitCapacity() {
        testsRun++;
        Stack s = new Stack(4);
        if (s.capacity() != 4) {
            fail("constructor uses given capacity", "wrong capacity");
            return;
        }
        if (s.size() != 0) {
            fail("constructor uses given capacity", "size != 0");
            return;
        }
        pass("constructor uses given capacity");
    }

    private static void testZeroCapacityThrows() {
        testsRun++;
        try {
            new Stack(0);
            fail("capacity 0 throws exception", "no exception");
            return;
        } catch (IllegalArgumentException e) {}
        pass("capacity 0 throws exception");
    }

    private static void testNegativeCapacityThrows() {
        testsRun++;
        try {
            new Stack(-1);
            fail("negative capacity throws exception", "no exception");
            return;
        } catch (IllegalArgumentException e) {}
        pass("negative capacity throws exception");
    }

    private static void testPushAndPop() {
        testsRun++;
        Stack s = new Stack(2);
        s.push("alpha");
        s.push("beta");
        s.push("gamma");

        if (s.size() != 3) {
            fail("pop returns elements in LIFO order", "size != 3");
            return;
        }
        if (!"gamma".equals(s.pop())) {
            fail("pop returns elements in LIFO order", "expected gamma");
            return;
        }
        if (!"beta".equals(s.pop())) {
            fail("pop returns elements in LIFO order", "expected beta");
            return;
        }
        if (!"alpha".equals(s.pop())) {
            fail("pop returns elements in LIFO order", "expected alpha");
            return;
        }
        if (!s.isEmpty()) {
            fail("pop returns elements in LIFO order", "not empty");
            return;
        }
        pass("pop returns elements in LIFO order");
    }

    private static void testPeek() {
        testsRun++;
        Stack s = new Stack();
        s.push("only");
        if (!"only".equals(s.peek())) {
            fail("peek shows top without removing it", "wrong value");
            return;
        }
        if (s.size() != 1) {
            fail("peek shows top without removing it", "size changed");
            return;
        }
        pass("peek shows top without removing it");
    }

    private static void testPopEmptyThrows() {
        testsRun++;
        try {
            new Stack().pop();
            fail("pop on empty stack throws exception", "no exception");
            return;
        } catch (EmptyStackException e) {}
        pass("pop on empty stack throws exception");
    }

    private static void testPeekEmptyThrows() {
        testsRun++;
        try {
            new Stack().peek();
            fail("peek on empty stack throws exception", "no exception");
            return;
        } catch (EmptyStackException e) {}
        pass("peek on empty stack throws exception");
    }

    private static void testPushNullThrows() {
        testsRun++;
        try {
            new Stack().push(null);
            fail("push null throws exception", "no exception");
            return;
        } catch (NullPointerException e) {}
        pass("push null throws exception");
    }

    private static void testPushCopiesInput() {
        testsRun++;
        Stack s = new Stack();
        String original = new StringBuilder("secret").toString();
        s.push(original);
        if (!"secret".equals(s.peek())) {
            fail("push stores a copy of the string", "data changed");
            return;
        }
        pass("push stores a copy of the string");
    }

    private static void testPeekReturnsCopy() {
        testsRun++;
        Stack s = new Stack();
        s.push("data");
        String a = s.peek();
        String b = s.peek();
        if (a == b) {
            fail("peek returns a new copy each call", "same reference");
            return;
        }
        if (!a.equals(b)) {
            fail("peek returns a new copy each call", "different content");
            return;
        }
        pass("peek returns a new copy each call");
    }

    private static void testClear() {
        testsRun++;
        Stack s = new Stack();
        s.push("a");
        s.push("b");
        s.clear();
        if (!s.isEmpty()) {
            fail("clear removes all elements", "not empty");
            return;
        }
        if (s.size() != 0) {
            fail("clear removes all elements", "size != 0");
            return;
        }
        pass("clear removes all elements");
    }

    private static void testExpansion() {
        testsRun++;
        Stack s = new Stack(2);
        for (int i = 0; i < 20; i++) {
            s.push("item_" + i);
        }
        if (s.size() != 20) {
            fail("stack grows when full", "size != 20");
            return;
        }
        if (s.capacity() < 20) {
            fail("stack grows when full", "capacity < 20");
            return;
        }
        for (int i = 19; i >= 0; i--) {
            String expected = "item_" + i;
            if (!expected.equals(s.pop())) {
                fail("stack grows when full", "wrong LIFO order");
                return;
            }
        }
        pass("stack grows when full");
    }

    private static void testPushEmptyString() {
        testsRun++;
        Stack s = new Stack();
        s.push("");
        if (!"".equals(s.pop())) {
            fail("push and pop with empty string works", "wrong value");
            return;
        }
        pass("push and pop with empty string works");
    }

    public static void main(String[] args) {
        System.out.println("\nTests:\n");

        testDefaultConstruction();
        testExplicitCapacity();
        testZeroCapacityThrows();
        testNegativeCapacityThrows();
        testPushAndPop();
        testPeek();
        testPopEmptyThrows();
        testPeekEmptyThrows();
        testPushNullThrows();
        testPushCopiesInput();
        testPeekReturnsCopy();
        testClear();
        testExpansion();
        testPushEmptyString();

        System.out.println("\nResults: " + testsPassed + " / " + testsRun + " passed \n");

        System.exit(testsPassed == testsRun ? 0 : 1);
    }
}
