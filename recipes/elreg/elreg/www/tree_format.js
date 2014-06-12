// Tree format definition
var TREE_FORMAT = [
	//  0. left position
	0,
	//  1. top position
	0,
	//  2. show buttons ("+" and "-" images)
	false,
	//  3. button images: collapsed state, expanded state, blank image
	["/images/blank.gif", "/images/blank.gif", "/images/blank.gif"],
	//  4. size of buttons: width, height, indent amount for childless nodes
	[16, 16, 0],
	//  5. show icons ("folder" and "document")
	true,
	//  6. icon images: closed folder, opened folder, document
	["/images/folder.png", "/images/folder_page.png", "/images/page.png"],
	//  7. size of icons: width, height
	[16, 16],
	//  8. indent amount for each level of the tree
	[0, 16, 32, 48, 64, 80, 96, 112, 128],
	//  9. background color for the tree
	"",
	// 10. default CSS class for nodes
	"treeNode",
	// 11. individual CSS classes for levels of the tree
	[null, null, null, null, null, null, null, null, null], 
	// 12. "single branch" mode
	false,
	// 13. padding and spacing values for all nodes
	[2, 0],
	// 14. "explorer-like" mode
	false,
	// 15. images for "explorer-like" mode
	["", "", "", "", "", "", "", "", "", ""],
	// 16. size of images for "explorer-like" mode: width, height
	[0, 0],
	// 17. store tree state into cookies
	true,
	// 18. relative positioning mode
	true,
	// 19. initial space for the relatively positioned tree: width, height
	[200, 50],
	// 20. resize container of the relatively positioned tree
	true,
	// 21. change background-color and style for selected node
	true,
	// 22. background color for unselected node, background color for selected node, class for selected node
	["#E4E4E4", "#E4E4E4", "clsNodeSelected"],
	// 23. text wrapping margin
	0,
	// 24. vertical alignment for buttons and icons
	"middle"
];
