import java.util.*;
import java.io.File;
import java.io.FileNotFoundException;

// compile: javac count_words.java
// run:     java count_words leipzig1M.txt 8 [optional: 10]
// test data file: https://algs4.cs.princeton.edu/31elementary/leipzig1M.txt
public class count_words {
    public static void main(String[] args) {
        try{
            long startTime = System.nanoTime();

            int minLen, k;
            if      (args.length < 2) { minLen = 1; k = 10; }
            else if (args.length < 3) { minLen = Integer.parseInt(args[1]); k = 10; }
            else    { minLen = Integer.parseInt(args[1]); k = Integer.parseInt(args[2]); }

            String text = new Scanner( new File(args[0]) ).useDelimiter("\\A").next();
            String[] words = text.toLowerCase().split("\\s+");
            HashMap<String, Integer> wordCounts = new HashMap<String, Integer>();

            for (String word : words) {
                if(word.length() < minLen) continue;
                Integer count = wordCounts.get(word);
                if (count == null) { // or we should use AtomicInteger instead
                    count = 0;
                }
                wordCounts.put(word, count + 1);
            }

            Iterator<Map.Entry<String, Integer>> itr = wordCounts.entrySet().iterator();
            showMostCommonWords(itr, k);

            long endTime = System.nanoTime();
            double duration = (endTime - startTime)/1e9;
            System.out.println("\nUsed "+duration+"s to find the top "+k+" most common words");
        }
        catch(NumberFormatException ex){
            System.out.println(ex.getMessage());
        }
        catch(FileNotFoundException ignored){
        }
    }

    public static void showMostCommonWords(Iterator<Map.Entry<String,Integer>> itr, int k)
    {
        Comparator<Map.Entry> cmp = new HeapComparator();
        PriorityQueue<Map.Entry> minHeap = new PriorityQueue<Map.Entry>(cmp);

        for(int i = 0; i < k && itr.hasNext(); ++i)
            minHeap.add(itr.next());

        Map.Entry curr;
        while(itr.hasNext()) {
            curr =  itr.next();
            if(cmp.compare(minHeap.peek(), curr) > 0)
                continue;
            else {
                minHeap.poll();
                minHeap.add(curr);
            }
        }

        Map.Entry[] arr = new Map.Entry[k];

        int i = k;
        while (!minHeap.isEmpty()) {
            arr[--i] = minHeap.peek();
            minHeap.poll();
        }

        for(Map.Entry pair : arr) {
            System.out.println(pair.getKey()
                    + " :  " + pair.getValue());
        }
    }
}

class HeapComparator implements Comparator<Map.Entry> {
    public int compare(Map.Entry lhs, Map.Entry rhs) {
        return (int)lhs.getValue() - (int)rhs.getValue();
    }
}
