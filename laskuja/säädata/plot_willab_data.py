from parse_willab_data import get_data

from plot_data import makeplot, monthly, no_locator_set


if __name__ == "__main__":
    makeplot(get_data(), monthly, step = 2)
