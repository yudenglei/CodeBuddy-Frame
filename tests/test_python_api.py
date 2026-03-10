#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
CAE PCB Plugin System - Python Test Script
==========================================

This script demonstrates how to use the CAE PCB Plugin System from Python.
It tests the following features:
1. Get Application instance
2. Get ActionManager instance
3. List all registered actions
4. Invoke existing actions
5. Register custom actions with Python callbacks
6. Update keyboard shortcuts

Usage:
    1. Build the project with ENABLE_PYTHON=ON
    2. Add build/python to PYTHONPATH or copy cae_plugin.pyd to this directory
    3. Run: python test_python_api.py
"""

import sys
import os

# Add the python build directory to path
# Adjust this path if needed
PYTHON_BUILD_DIR = os.path.join(os.path.dirname(__file__), "build", "python")
if os.path.exists(PYTHON_BUILD_DIR):
    sys.path.insert(0, PYTHON_BUILD_DIR)

try:
    import cae_plugin
except ImportError as e:
    print(f"Error: Could not import cae_plugin module: {e}")
    print(f"\nPlease ensure:")
    print(f"  1. The project is built with ENABLE_PYTHON=ON")
    print(f"  2. The cae_plugin.pyd file is in the Python path")
    print(f"  3. PYTHONPATH includes: {PYTHON_BUILD_DIR}")
    sys.exit(1)


def test_basic_usage():
    """Test basic usage of the CAE PCB Plugin System"""
    print("=" * 60)
    print("Test 1: Basic Usage")
    print("=" * 60)
    
    # Get Application singleton
    app = cae_plugin.Application.instance()
    print(f"Application version: {app.version()}")
    
    # Get ActionManager
    am = app.action_manager()
    print("Got ActionManager instance")
    
    return app, am


def test_list_actions(am):
    """Test listing all registered actions"""
    print("\n" + "=" * 60)
    print("Test 2: List All Registered Actions")
    print("=" * 60)
    
    actions = am.list_actions()
    print(f"Total actions registered: {len(actions)}")
    print("\nAction list:")
    print("-" * 60)
    
    # Group actions by menu
    menus = {}
    for action in actions:
        menu_path = action.menu_path if action.menu_path else "No Menu"
        if menu_path not in menus:
            menus[menu_path] = []
        menus[menu_path].append(action)
    
    # Print grouped by menu
    for menu, acts in sorted(menus.items()):
        print(f"\n[{menu}]")
        for a in acts:
            shortcut = f" ({a.shortcut})" if a.shortcut else ""
            print(f"  - {a.id}: {a.label}{shortcut}")
    
    return actions


def test_invoke_action(am):
    """Test invoking existing actions"""
    print("\n" + "=" * 60)
    print("Test 3: Invoke Existing Actions")
    print("=" * 60)
    
    # Test invoking File.New
    print("\nInvoking 'File.New'...")
    am.invoke("File.New")
    
    # Test invoking Edit.Undo
    print("Invoking 'Edit.Undo'...")
    am.invoke("Edit.Undo")
    
    # Test invoking with parameters
    print("Invoking 'Tools.RunScript' with params...")
    am.invoke("Tools.RunScript", {"script_path": "/tmp/test.py"})
    
    print("\nAction invocation test completed!")


def test_register_action(am):
    """Test registering custom actions with Python callbacks"""
    print("\n" + "=" * 60)
    print("Test 4: Register Custom Actions")
    print("=" * 60)
    
    # Register a simple action
    print("\nRegistering 'Python.Test.Hello'...")
    am.register_action(
        "Python.Test.Hello",
        "Hello from Python",
        lambda: print("  >>> Hello action executed!"),
        "Tools"
    )
    
    # Register an action with more complex logic
    print("Registering 'Python.Test.Complex'...")
    counter = [0]  # Use list to avoid nonlocal issue
    def complex_callback():
        counter[0] += 1
        print(f"  >>> Complex action executed! Count: {counter[0]}")
    
    am.register_action(
        "Python.Test.Complex",
        "Complex Python Action",
        complex_callback,
        "Tools"
    )
    
    # Invoke the registered actions
    print("\nInvoking 'Python.Test.Hello'...")
    am.invoke("Python.Test.Hello")
    
    print("\nInvoking 'Python.Test.Complex' multiple times...")
    for i in range(3):
        am.invoke("Python.Test.Complex")
    
    print("\nAction registration test completed!")


def test_update_shortcut(am):
    """Test updating keyboard shortcuts"""
    print("\n" + "=" * 60)
    print("Test 5: Update Keyboard Shortcuts")
    print("=" * 60)
    
    # Update shortcut for an existing action
    print("\nUpdating shortcut for 'File.New' to 'Ctrl+Shift+N'...")
    am.set_shortcut("File.New", "Ctrl+Shift+N")
    
    # List actions to verify the change
    actions = am.list_actions()
    for action in actions:
        if action.id == "File.New":
            print(f"  New shortcut: {action.shortcut}")
            break
    
    print("\nShortcut update test completed!")


def test_plugin_interaction(am):
    """Test interacting with plugins"""
    print("\n" + "=" * 60)
    print("Test 6: Plugin Interaction")
    print("=" * 60)
    
    # Test PCB DB plugin actions
    print("\nTesting PCB DB plugin actions:")
    print("  Invoking 'Layout.RunDRC'...")
    am.invoke("Layout.RunDRC")
    
    print("  Invoking 'Layout.LayerManager'...")
    am.invoke("Layout.LayerManager")
    
    # Test PCB Analysis plugin actions
    print("\nTesting PCB Analysis plugin actions:")
    print("  Invoking 'Tools.ImpedanceAnalysis'...")
    am.invoke("Tools.ImpedanceAnalysis")
    
    print("\nPlugin interaction test completed!")


def main():
    """Main test function"""
    print("\n" + "=" * 60)
    print("CAE PCB Plugin System - Python API Test")
    print("=" * 60)
    
    try:
        # Test 1: Basic usage
        app, am = test_basic_usage()
        
        # Test 2: List actions
        test_list_actions(am)
        
        # Test 3: Invoke actions
        test_invoke_action(am)
        
        # Test 4: Register custom actions
        test_register_action(am)
        
        # Test 5: Update shortcuts
        test_update_shortcut(am)
        
        # Test 6: Plugin interaction
        test_plugin_interaction(am)
        
        print("\n" + "=" * 60)
        print("All tests completed successfully!")
        print("=" * 60)
        
    except Exception as e:
        print(f"\nError during tests: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)


if __name__ == "__main__":
    main()
