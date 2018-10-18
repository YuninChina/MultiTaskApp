
for i in `find -maxdepth 3 -type d | grep -v git `;do rm -f $i/*.o $i/*.bak $i/*.so $i/lib ;done
rm -f make_app

