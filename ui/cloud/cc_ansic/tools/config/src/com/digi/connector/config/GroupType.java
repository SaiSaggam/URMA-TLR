package com.digi.connector.config;

import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class GroupType {

    private static final Map<String, GroupType> TYPE_MAP = new LinkedHashMap<String, GroupType>();
    private static final List<GroupType> TYPE_LIST = new ArrayList<GroupType>();

    public static final GroupType SETTING = build(0,"SETTING");
    public static final GroupType STATE = build(1,"STATE");

    private final int index;
    private final String type;

    public GroupType(int index,String type){
        this.index = index;	
        this.type = type;			
    }

    private static GroupType build( final int index,final String type) {
        final GroupType group= new GroupType(index, type);
        TYPE_MAP.put(type, group);
        TYPE_LIST.add(group);
        return group;
    }

    public int getIndex() {
        return index;
    }

    public static GroupType toGroupType(final String type) {
        if (type == null) {
            throw new NullPointerException();
        }
        final GroupType group = TYPE_MAP.get(type.toUpperCase());
        if (group == null) {
            throw new IllegalArgumentException();
        }
        return group;
    }

    public static GroupType[] values(){

        return TYPE_LIST.toArray(new GroupType[TYPE_LIST.size()]);  	
    }

    @Override
    public String toString() {
        return type;
    }

}
