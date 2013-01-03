from parse_cetml1659on import get_data

from plot_data import makeplot, monthly, no_locator_set


if __name__ == "__main__":
    makeplot(get_data(), no_locator_set, step = 2)
    
