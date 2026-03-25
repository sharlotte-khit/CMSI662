import java.util.Arrays;
import java.util.EmptyStackException;

public class Stack {

    public static final int MAX_STRING_LENGTH = 1048576;
    public static final int MAX_CAPACITY = 1048576;
    public static final int DEFAULT_CAPACITY = 16;

    private static final int GROWTH_FACTOR = 2;

    private String[] data;
    private int size;

    public Stack() {
        this(DEFAULT_CAPACITY);
    }

    public Stack(int initialCapacity) {
        if (initialCapacity <= 0 || initialCapacity > MAX_CAPACITY) {
            throw new IllegalArgumentException(
                    "Initial capacity must be in [1, " + MAX_CAPACITY + "]; got " + initialCapacity);
        }
        this.data = new String[initialCapacity];
        this.size = 0;
    }

    public void push(String str) {
        if (str == null) {
            throw new NullPointerException("Cannot push a null string");
        }
        if (str.length() > MAX_STRING_LENGTH) {
            throw new IllegalArgumentException(
                    "String length " + str.length() + " exceeds maximum of " + MAX_STRING_LENGTH);
        }

        if (size >= data.length) {
            expand();
        }

        data[size] = new String(str);
        size++;
    }

    public String pop() {
        if (size == 0) {
            throw new EmptyStackException();
        }

        size--;
        String value = data[size];
        data[size] = null;
        return value;
    }

    public String peek() {
        if (size == 0) {
            throw new EmptyStackException();
        }
        return new String(data[size - 1]);
    }

    public int size() {
        return size;
    }

    public int capacity() {
        return data.length;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public void clear() {
        Arrays.fill(data, 0, size, null);
        size = 0;
    }

    private void expand() {
        if (data.length >= MAX_CAPACITY) {
            throw new IllegalStateException(
                    "Stack has reached the maximum permitted capacity of " + MAX_CAPACITY);
        }

        long newCapacity = (long) data.length * GROWTH_FACTOR;
        if (newCapacity > MAX_CAPACITY) {
            newCapacity = MAX_CAPACITY;
        }

        data = Arrays.copyOf(data, (int) newCapacity);
    }
}
