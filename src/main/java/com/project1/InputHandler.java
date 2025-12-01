package com.project1;

import java.util.HashSet;
import java.util.Set;

public class InputHandler {
    
    // A HashSet prevents duplicate keys (e.g., can't have "W" twice)
    private final Set<String> activeInputs = new HashSet<>();

    public boolean addKey(String key) {
        if (key == null) return false;
        return activeInputs.add(key);
    }

    public boolean removeKey(String key) {
        if (key == null) return false;
        return activeInputs.remove(key);
    }

    public Set<String> getActiveInputs() {
        return new HashSet<>(activeInputs);
    }

    public boolean isEmpty() {
        return activeInputs.isEmpty();
    }
}